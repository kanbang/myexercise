#include "stdafx.h"
#include "InstrumentDlg.h"

#include "SimpleInputDlg.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

IMPLEMENT_DYNAMIC( InstrumentDlg, CDialog )

InstrumentDlg::InstrumentDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( InstrumentDlg::IDD, pParent )
    , m_type( _T( "" ) )
    , m_num( _T( "" ) )
    , m_factory( _T( "" ) )
    , m_institute( _T( "" ) )
    , m_ok( FALSE )
    //, m_name(_T(""))
{

}

InstrumentDlg::~InstrumentDlg()
{
}

void InstrumentDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_INSTRUMENT_LIST, m_list );
    //DDX_Text(pDX, IDC_INSTRUMENT_NAME, m_name);
    DDX_Text( pDX, IDC_INSTRUMENT_TYPE, m_type );
    DDX_Text( pDX, IDC_INSTRUMENT_NUM, m_num );
    DDX_Text( pDX, IDC_INSTRUMENT_FACTORY, m_factory );
    DDX_Text( pDX, IDC_INSTRUMENT_INSTITUTE, m_institute );
    DDX_Radio( pDX, IDC_INSTRUMENT_RADIO1, m_ok );
}

BEGIN_MESSAGE_MAP( InstrumentDlg, CDialog )
    ON_WM_CLOSE()
    ON_NOTIFY( LVN_ITEMCHANGED, IDC_INSTRUMENT_LIST, &InstrumentDlg::OnLvnItemchangedInstrumentList )
    ON_BN_CLICKED( IDC_INSTRUMENT_ADD_BTN, &InstrumentDlg::OnBnClickedInstrumentAddBtn )
    ON_BN_CLICKED( IDC_INSTRUMENT_DEL_BTN, &InstrumentDlg::OnBnClickedInstrumentDelBtn )
END_MESSAGE_MAP()


// InstrumentDlg 消息处理程序

BOOL InstrumentDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_list.SetExtendedStyle( m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
    CRect rect;
    m_list.GetClientRect( &rect );
    double c = 0.3;
    m_list.InsertColumn( 0, _T( "序号" ), LVCFMT_LEFT, rect.Width()*c );
    m_list.InsertColumn( 1, _T( "名称" ), LVCFMT_LEFT, rect.Width() * ( 1 - c ) );

    clearAll();
    initList();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void InstrumentDlg::OnClose()
{
    updateLastSelect();

    CDialog::OnClose();
}

void InstrumentDlg::OnLvnItemchangedInstrumentList( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );

    if( pNMLV->iItem != -1 )
    {
        // 第几个图元
        int row = pNMLV->iItem;

        if( m_lastPos != -1 )
        {
            // 保存数据
            UpdateData( TRUE );
            writePropertyData( m_objIds[m_lastPos] );
        }
        // 读取当前项的图元的数据
        readPropertyData( m_objIds[row] );
        UpdateData( FALSE ); // 更新数据到界面

        m_lastPos = row;
    }

    *pResult = 0;
}

void InstrumentDlg::initList()
{
    AcStringArray names;
    DataObjectListHelper::GetObjectNameList( _T( "安全检测仪表" ), names );

    int len = names.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObjectId objId;
        DataObjectListHelper::GetObjectId( _T( "安全检测仪表" ), names[i].kACharPtr(), objId );
        m_objIds.append( objId );

        // 序号
        CString num;
        num.Format( _T( "%d" ), i + 1 );
        m_list.InsertItem( i, num );
        //m_list.SetItemData(i, i);

        m_list.SetItemText( i, 0, num );
        m_list.SetItemText( i, 1, names[i].kACharPtr() );
    }
    // 选中第1个
    if( len > 0 ) m_list.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );
}

void InstrumentDlg::updateLastSelect()
{
    int row = LB_ERR;
    for( int i = 0; i < m_list.GetItemCount(); i++ )
    {
        if( LVIS_SELECTED == m_list.GetItemState( i, LVIS_SELECTED ) )
        {
            row = i;
            break;
        }
    }

    if( row != LB_ERR )
    {
        UpdateData( TRUE );
        writePropertyData( m_objIds[row] );
    }
}

void InstrumentDlg::readPropertyData( const AcDbObjectId& objId )
{
    //DataHelper::GetPropertyData(objId, _T("名称"), m_name);
    DataHelper::GetPropertyData( objId, _T( "型号" ), m_type );
    DataHelper::GetPropertyData( objId, _T( "数量" ), m_num );
    DataHelper::GetPropertyData( objId, _T( "生产单位" ), m_factory );
    DataHelper::GetPropertyData( objId, _T( "计量检验单位" ), m_institute );

    CString v2;
    DataHelper::GetPropertyData( objId, _T( "计量检验结果" ), v2 );
    int i2 = 0;
    ArxUtilHelper::StringToInt( v2, i2 );
    i2  = i2 % 2;
    m_ok = i2; // 0-不合格，1-合格
}

void InstrumentDlg::writePropertyData( const AcDbObjectId& objId )
{
    //DataHelper::SetPropertyData(objId, _T("名称"), m_name);
    DataHelper::SetPropertyData( objId, _T( "型号" ), m_type );
    DataHelper::SetPropertyData( objId, _T( "数量" ), m_num );
    DataHelper::SetPropertyData( objId, _T( "生产单位" ), m_factory );
    DataHelper::SetPropertyData( objId, _T( "计量检验单位" ), m_institute );

    CString v2;
    v2.Format( _T( "%d" ), m_ok ); // 0-不合格，1-合格
    DataHelper::SetPropertyData( objId, _T( "计量检验结果" ), v2 );
}

void InstrumentDlg::clearAllData()
{
    m_type = _T( "" );
    m_num = _T( "" );
    m_factory = _T( "" );
    m_institute = _T( "" );
    m_ok = FALSE;
    //m_name = _T("");
}

void InstrumentDlg::clearAll()
{
    m_list.DeleteAllItems();
    m_objIds.removeAll();
    m_lastPos = -1;

    clearAllData();
}

static bool GetInputName( CString& name )
{
    // 切换资源
    CAcModuleResourceOverride myResources;

    SimpleInputDlg sidlg;
    if( sidlg.DoModal() != IDOK ) return false;

    name = sidlg.m_str;
    return true;
}

void InstrumentDlg::OnBnClickedInstrumentAddBtn()
{
    CString name;
    if( !GetInputName( name ) ) return;

    DataObjectListHelper::AddObject( _T( "安全检测仪表" ), name );

    // 采用比较笨的方法，每次点击"增加"按钮都清空list，并插入数据
    updateLastSelect();
    clearAll();
    initList();
}

void InstrumentDlg::OnBnClickedInstrumentDelBtn()
{
    int row = LB_ERR;
    for( int i = 0; i < m_list.GetItemCount(); i++ )
    {
        if( LVIS_SELECTED == m_list.GetItemState( i, LVIS_SELECTED ) )
        {
            row = i;
            break;
        }
    }
    CString name = m_list.GetItemText( row, 1 );
    DataObjectListHelper::RemoveObject( _T( "安全检测仪表" ), name );
    clearAll();
    initList();
}
