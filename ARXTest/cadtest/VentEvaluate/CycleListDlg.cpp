#include "stdafx.h"
#include "CycleListDlg.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

#include "../VentNetworkAlgo/VNAlgoHelper.h"

IMPLEMENT_DYNAMIC( CycleListDlg, CAcUiDialog )

CycleListDlg::CycleListDlg( CWnd* pParent, HINSTANCE hInstance )
    : CAcUiDialog( CycleListDlg::IDD, pParent, hInstance )
{

}

CycleListDlg::~CycleListDlg()
{
}

void CycleListDlg::DoDataExchange( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_CYCLE_LIST, m_list );
}


BEGIN_MESSAGE_MAP( CycleListDlg, CAcUiDialog )
    ON_BN_CLICKED( IDC_CYCLE_UPDATE_BTN, &CycleListDlg::OnBnClickedCycleListUpdateBtn )
    ON_NOTIFY( LVN_ITEMCHANGED, IDC_CYCLE_LIST, &CycleListDlg::OnLvnItemchangedList1 )
END_MESSAGE_MAP()


//- Needed for modeless dialogs to keep focus.
//- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CycleListDlg::OnAcadKeepFocus ( WPARAM wParam, LPARAM lParam )
{
    return ( TRUE ) ;
}

//- As this dialog is a child dialog we need to disable ok and cancel
BOOL CycleListDlg::OnCommand ( WPARAM wParam, LPARAM lParam )
{
    switch ( wParam )
    {
    case IDOK:
    case IDCANCEL:
        return ( FALSE );
    }
    return ( CAcUiDialog::OnCommand ( wParam, lParam ) );
}

void CycleListDlg::OnSize ( UINT nType, int cx, int cy )
{
    CAcUiDialog::OnSize ( nType, cx, cy ) ;
    //- Now update the dialog
    MoveWindow( 0, 0, cx, cy );
}

// CycleListDlg ��Ϣ��������

void CycleListDlg::OnBnClickedCycleListUpdateBtn()
{
    // ���
    clearAll();

    // ��ʼ��list
    initTunnelList();
}

void CycleListDlg::OnLvnItemchangedList1( NMHDR* pNMHDR, LRESULT* pResult )
{
    if( !m_listFinishInitial ) return;

    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );

    if( pNMLV->iItem != -1 )
    {
        // ������ǰ�ĵ�
        acDocManager->lockDocument( curDoc() );

        // �ڼ���ͼԪ
        int row = pNMLV->iItem;

        if( m_lastPos != -1 )
        {
            int startPos, endPos;
            findEdgeByCC( m_lastPos, startPos, endPos );
            for( int i = startPos; i < endPos; i++ )
            {
                ArxEntityHelper::SetEntityColor( m_objIds[i], m_colors[i] );
            }
        }

        // ���Ų��û���ɫ������ʾ
        int startPos, endPos;
        findEdgeByCC( row, startPos, endPos );
        AcDbObjectIdArray objIds;
        for( int i = startPos; i < endPos; i++ )
        {
            objIds.append( m_objIds[i] );
        }
        ArxEntityHelper::ZoomToEntities( objIds );
        ArxEntityHelper::SetEntitiesColor( objIds, 2 );

        m_lastPos = row;

        acDocManager->unlockDocument( curDoc() );
    }

    *pResult = 0;
}

BOOL CycleListDlg::OnInitDialog()
{
    CAcUiDialog::OnInitDialog();

    // ����listctrl�ı���ͷ
    m_list.SetExtendedStyle( m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
    CRect rect;
    m_list.GetClientRect( &rect );
    double c = 0.3;
    m_list.InsertColumn( 0, _T( "�����·���" ), LVCFMT_LEFT, rect.Width()/**c*/ );
    //m_list.InsertColumn(1, _T(""), LVCFMT_LEFT, rect.Width()*(1-c));

    // ���
    clearAll();

    initTunnelList();

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CycleListDlg::initTunnelList()
{
    // ������ǰ�ĵ�
    acDocManager->lockDocument( curDoc() );

    AcDbObjectIdArray objIds;
    AcDbIntArray sccIds;
    bool ret = ConnectivityHelper::SCC( m_objIds, sccIds );

    // ������ǰ�ĵ�
    acDocManager->unlockDocument( curDoc() );

    if( !ret )
    {
        MessageBox( _T( "��ʼ������ʧ��" ) );
        return;
    }
    if( sccIds.isEmpty() )
    {
        MessageBox( _T( "ͨ��������û�е����·" ) );
        return;
    }

    m_objIds.append( objIds );
    m_sccIds.append( sccIds );
    ArxEntityHelper::GetEntitiesColor( m_objIds, m_colors ); 	// ��¼��ɫ

    int len = sccIds.length();
    for ( int i = 0; i < len; i++ )
    {
        // ���
        CString num;
        num.Format( _T( "��·%d" ), i + 1 );
        m_list.InsertItem( i, num );
        //m_list.SetItemData(i, i);

        m_list.SetItemText( i, 0, num );
    }

    // ѡ�е�1��
    if( len > 0 )
    {
        m_list.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );
        m_listFinishInitial = true;
    }
}

// Ŀǰ�����д�����
void CycleListDlg::findEdgeByCC( int cc, int& startPos, int& endPos )
{
    int len = m_sccIds.length();
    int pos = 0;
    for( int i = 0; i < len; i++ )
    {
        if( i == cc )
        {
            startPos = pos;
            endPos = startPos + m_sccIds[i];
            break;
        }
        else
        {
            pos = pos + m_sccIds[i];
        }
    }
}

void CycleListDlg::clearAll()
{
    // ������ǰ�ĵ�
    acDocManager->lockDocument( curDoc() );

    // �ָ���ɫ
    ArxEntityHelper::SetEntitiesColor2( m_objIds, m_colors );

    acDocManager->unlockDocument( curDoc() );

    m_list.DeleteAllItems();
    m_objIds.removeAll();
    m_colors.removeAll();
    m_sccIds.removeAll();
    m_lastPos = -1;
    m_listFinishInitial = false;
}

void CycleListDlg::OnClosing()
{
    // ���
    clearAll();
}