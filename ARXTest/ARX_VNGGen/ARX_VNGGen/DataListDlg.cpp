#include "stdafx.h"
#include "DataListDlg.h"

#include "MVSSData.h"
#include "../ArxHelper/HelperClass.h"

/* 实现在ARX_VNGGen.cpp */
extern void GetAllFields( const CString& type, AcStringArray& fields );
extern void GetAllDatas( const AcDbObjectId& objId, AcStringArray& values );

struct ItemData
{
    int iItem;            // 第几行的数据
    AcDbObjectId objId;   // 数据id
};

// 用于CtrlList的排序函数
struct SortData
{
    int iSubItem;
    CListCtrl* pList;
};

static int nSortOrder = 1;      // 排序的方式,1升序，-1降序。

int CALLBACK SortFunc( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
    ItemData* pData1 = ( ItemData* )lParam1;
    ItemData* pData2 = ( ItemData* )lParam2;
    SortData* pSD = ( SortData* )lParamSort;

    // 采用最简单的办法，不考虑具体是哪一栏的数据
    // 按照字符串排序即可

    CString s1 = pSD->pList->GetItemText( pData1->iItem, pSD->iSubItem );
    CString s2 = pSD->pList->GetItemText( pData2->iItem, pSD->iSubItem );

    int nRetVal = 0;

    if( pSD->iSubItem == 0 )
    {
        // 第1栏(编号)
        int n1 = _ttoi( s1 );
        int n2 = _ttoi( s2 );
        nRetVal = ( ( n1 < n2 ) ? -1 : 1 );
    }
    else
    {
        nRetVal = s1.CompareNoCase( s2 );
    }

    return nRetVal * nSortOrder;
}

static void AdjustItemDataNum( CListCtrl& m_list )
{
    // 因为排序可能导致iItem与实际的行位置不一致，需要调整
    int n = m_list.GetItemCount();
    for( int i = 0; i < n; i++ )
    {
        ItemData* pData = ( ItemData* )m_list.GetItemData( i );
        pData->iItem = i;
    }
}

static int FindItemByObjectId( CListCtrl& m_list, const AcDbObjectId& objId )
{
    int row = CB_ERR;
    int n = m_list.GetItemCount();
    for( int i = 0; i < n; i++ )
    {
        ItemData* pData = ( ItemData* )m_list.GetItemData( i );
        if( pData->objId == objId )
        {
            row = i;
            break;
        }
    }
    return row;
}

static void ClearList( CListCtrl& m_list )
{
    //首先删除所有行
    m_list.DeleteAllItems();
    //得到列数
    int n = m_list.GetHeaderCtrl()->GetItemCount();
    //删除所有列
    for( int i = 0; i < n; i++ )
    {
        m_list.DeleteColumn( 0 );
    }
}

static void BuildHeadColumns( CListCtrl& m_list, const AcStringArray fields )
{
    int len = fields.length();

    CRect rect;
    m_list.GetClientRect( &rect );

    int n = len;
    if( n > 10 ) n = 10;
    double dw = rect.Width() / ( n + 1 );

    m_list.InsertColumn( 0, _T( "编号" ), LVCFMT_LEFT, dw );
    for( int i = 0; i < len; i++ )
    {
        m_list.InsertColumn( i + 1, fields[i].kACharPtr(), LVCFMT_LEFT, dw );
    }
}

static void FillDatas( CListCtrl& m_list, const AcStringArray fields, const AcDbObjectIdArray& objIds )
{
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcStringArray values;
        GetAllDatas( objIds[i], values );
        if( values.isEmpty() ) continue;

        m_list.InsertItem( i, _T( "xx" ) );

        CString num;
        num.Format( _T( "%d" ), i + 1 );
        m_list.SetItemText( i, 0, num );

        ItemData* pData = new ItemData();
        pData->iItem = i;
        pData->objId = objIds[i];
        m_list.SetItemData( i, ( LPARAM )pData ); // 设置数据

        int n = values.length();
        for( int j = 0; j < n; j++ )
        {
            //m_list.SetItemData(i, i);
            m_list.SetItemText( i, j + 1, values[j].kACharPtr() );
        }
    }
}

static void ClearItemData( CListCtrl& m_list )
{
    int n = m_list.GetItemCount();
    for( int i = 0; i < n; i++ )
    {
        ItemData* pData = ( ItemData* )m_list.GetItemData( i );
        delete pData;
    }
}

/*
 * 帮助文档中acedSSSetFirst以及acedSSGetFirst
 * 要求使用它的命令应该开启ACRX_CMD_USEPICKSET和ACRX_CMD_REDRAW选项
 * 但测试结果显示，貌似不使用也可以??????
 */
static bool SelectEntity( const AcDbObjectId& objId )
{
    //acedSSSetFirst(NULL, NULL);

    if( objId.isNull() ) return false;

    ads_name ename;
    if( Acad::eOk != acdbGetAdsName( ename, objId ) ) return false;;

    ads_name ss;
    if( RTNORM != acedSSAdd( ename, NULL, ss ) ) return false; // 创建选择集

    acedSSSetFirst( ss, NULL ); // 高亮选中
    acedSSFree( ss );           // 释放选择集

    return true;
}

static void ZoomToEntity( const AcDbObjectId& objId )
{
    // 在非模态对话框下无法使用
    //ads_name en;
    //if(Acad::eOk != acdbGetAdsName(en, objId)) return;
    //acedCommand(RTSTR, _T("ZOOM"), RTSTR, _T("O"), RTENAME, en, RTSTR, _T(""), 0);

    // 临时使用sendStringToExecute解决缩放定位问题
    CString cmd;
    cmd.Format( _T( "ZOOM O \003" ) ); // 按空格结束选择对象，然后esc(防止多余的空格重复执行命令)
    acDocManager->sendStringToExecute( curDoc(), cmd, true, false, false );
}

IMPLEMENT_DYNAMIC( DataListDlg, DockBarChildDlg )

DataListDlg::DataListDlg( CWnd* pParent /*=NULL*/ )
    : DockBarChildDlg( DataListDlg::IDD, pParent )
{
}

DataListDlg::~DataListDlg()
{
}

void DataListDlg::DoDataExchange( CDataExchange* pDX )
{
    DockBarChildDlg::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_TYPE_COMBO, m_typeList );
    DDX_Control( pDX, IDC_DATA_LIST, m_geList );
}


BEGIN_MESSAGE_MAP( DataListDlg, DockBarChildDlg )
    ON_BN_CLICKED( IDC_UPDATE_BTN, &DataListDlg::OnBnClickedUpdateBtn )
    ON_BN_CLICKED( IDC_HILIGHT_SELECT_BTN, &DataListDlg::OnBnClickedHilightSelectBtn )
    ON_NOTIFY( LVN_ITEMCHANGED, IDC_DATA_LIST, &DataListDlg::OnLvnItemchangedDataList )
    ON_NOTIFY( LVN_COLUMNCLICK, IDC_DATA_LIST, &DataListDlg::OnLvnColumnclickGeList )
    ON_BN_CLICKED( IDC_SELECT_BTN, &DataListDlg::OnBnClickedSelectBtn )
END_MESSAGE_MAP()

// DataListDlg 消息处理程序

BOOL DataListDlg::OnInitDialog()
{
    DockBarChildDlg::OnInitDialog();

    m_geList.SetExtendedStyle( m_geList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

    m_typeList.AddString( _T( "节点" ) );
    m_typeList.AddString( _T( "分支" ) );
    m_typeList.AddString( _T( "构筑物" ) );
    m_typeList.AddString( _T( "通风动力" ) );

    return TRUE;
}


void DataListDlg::OnClosing()
{
    //m_typeList.ResetContent();
    ClearItemData( m_geList );
    ClearList( m_geList );
}

void DataListDlg::OnBnClickedUpdateBtn()
{
    // 删除所有的数据(item data)
    ClearItemData( m_geList );

    // 清空链表
    ClearList( m_geList );

    // 获取类型
    CString type = GetTypeName( ( MVSS_DATA_TYPE )m_typeList.GetCurSel() );

    acDocManager->lockDocument( curDoc() );

    // 获取所有的字段
    AcStringArray fields;
    GetAllFields( type, fields );

    // 构建Column
    BuildHeadColumns( m_geList, fields );

    // 查找所有类型=type的图元
    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( type, objIds );

    m_initFinished = false;

    // 填充数据
    FillDatas( m_geList, fields, objIds );

    acDocManager->unlockDocument( curDoc() );

    // 数据初始化完毕
    m_initFinished = true;
}

void DataListDlg::OnBnClickedHilightSelectBtn()
{
    int row = LB_ERR;
    for( int i = 0; i < m_geList.GetItemCount(); i++ )
    {
        if( LVIS_SELECTED == m_geList.GetItemState( i, LVIS_SELECTED ) )
        {
            row = i;
            break;
        }
    }
    if( row == LB_ERR )
    {
        MessageBox( _T( "请选择一条数据" ) );
    }
    else
    {
        ItemData* pData = ( ItemData* )m_geList.GetItemData( row );

        acDocManager->lockDocument( curDoc() );

        SelectEntity( pData->objId );

        acDocManager->unlockDocument( curDoc() );

        acedGetAcadFrame()->SetFocus();
    }
}

void DataListDlg::OnLvnItemchangedDataList( NMHDR* pNMHDR, LRESULT* pResult )
{
    if( !m_initFinished ) return;

    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );

    int row = pNMLV->iItem;
    if( row != LB_ERR )
    {
        ItemData* pData = ( ItemData* )m_geList.GetItemData( row );
        //acutPrintf(_T("\nid:%d"), pData->objId);

        acDocManager->lockDocument( curDoc() );

        if( SelectEntity( pData->objId ) )
        {
            ZoomToEntity( pData->objId );
        }

        acDocManager->unlockDocument( curDoc() );
    }

    *pResult = 0;
}

void DataListDlg::OnLvnColumnclickGeList( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );

    static int ncurSortCol = -1;      // 保存当前的排序列

    /*
     * 一开始表示为-1，表示尚未按任何列排序
     * 若当前排序列与点击列相同，则更改增、减序的排序方式。
     * 若点击列与当前排序列不同的列，则改变排序序，并将排序方式改为增序
     */
    if( ncurSortCol == pNMLV->iSubItem )
    {
        nSortOrder *= -1;
    }
    else
    {
        ncurSortCol = pNMLV->iSubItem;
        nSortOrder = 1;
    }
    AdjustItemDataNum( m_geList );

    SortData sd = {pNMLV->iSubItem, &m_geList};
    m_geList.SortItems( SortFunc, ( LPARAM )&sd ); // 排序

    *pResult = 0;
}

void DataListDlg::OnBnClickedSelectBtn()
{
    CString type = GetTypeName( ( MVSS_DATA_TYPE )m_typeList.GetCurSel() );
    CString name = GetTableName( ( MVSS_DATA_TYPE )m_typeList.GetCurSel() );

    CString msg;
    msg.Format( _T( "请选择一个%s: " ), name );
    AcDbObjectId objId = ArxUtilHelper::SelectObject( msg );

    if( objId.isNull() || !ArxUtilHelper::IsEqualType( type, objId ) )
    {
        MessageBox( _T( "拾取失败" ) );
    }
    else
    {
        int pos = FindItemByObjectId( m_geList, objId );
        if( pos != CB_ERR )
        {
            m_geList.EnsureVisible( pos, FALSE );
            m_geList.SetItemState( pos, LVIS_SELECTED, LVIS_SELECTED );
        }
    }
}
