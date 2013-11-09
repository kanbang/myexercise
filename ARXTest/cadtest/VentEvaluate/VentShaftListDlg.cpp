#include "stdafx.h"
#include "VentShaftListDlg.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

#include "ShaftExtraPropertyDlg.h"

/* ȫ�ֺ���(ʵ����Tool.cpp��) */
extern int FindShafts( AcDbObjectIdArray& shafts );

IMPLEMENT_DYNAMIC( VentShaftListDlg, CAcUiDialog )

VentShaftListDlg::VentShaftListDlg( CWnd* pParent, HINSTANCE hInstance )
    : CAcUiDialog( VentShaftListDlg::IDD, pParent, hInstance )
{

}

VentShaftListDlg::~VentShaftListDlg()
{
    // arx����ж�أ����Ի�����δ�ر�
    // ��ʱӦ�����´���
    if( curDoc() != 0 )
    {
        updateLastSelect();
        clearAll();
    }
}

void VentShaftListDlg::DoDataExchange( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_VENT_SHAFT_LIST, m_list );
    DDX_Text( pDX, IDC_SHAFT_NAME_EDIT, m_name );
    DDX_Text( pDX, IDC_SHAFT_AREA_EDIT, m_area );
    DDX_Text( pDX, IDC_SHAFT_VELOCITY_EDIT, m_velocity );
    DDX_Text( pDX, IDC_SHAFT_TEMP_EDIT, m_temp );
}


BEGIN_MESSAGE_MAP( VentShaftListDlg, CAcUiDialog )
    ON_BN_CLICKED( IDC_VENT_SHAFT_UPDATE_BTN, &VentShaftListDlg::OnBnClickedVentInShaftUpdateBtn )
    ON_NOTIFY( LVN_ITEMCHANGED, IDC_VENT_SHAFT_LIST, &VentShaftListDlg::OnLvnItemchangedList1 )
    ON_BN_CLICKED( IDC_VENT_SHAFT_PARAM_BTN, &VentShaftListDlg::OnBnClickedVentShaftParamBtn )
END_MESSAGE_MAP()


//- Needed for modeless dialogs to keep focus.
//- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT VentShaftListDlg::OnAcadKeepFocus ( WPARAM wParam, LPARAM lParam )
{
    return ( TRUE ) ;
}

//- As this dialog is a child dialog we need to disable ok and cancel
BOOL VentShaftListDlg::OnCommand ( WPARAM wParam, LPARAM lParam )
{
    switch ( wParam )
    {
    case IDOK:
    case IDCANCEL:
        return ( FALSE );
    }
    return ( CAcUiDialog::OnCommand ( wParam, lParam ) ) ;
}

void VentShaftListDlg::OnSize ( UINT nType, int cx, int cy )
{
    CAcUiDialog::OnSize ( nType, cx, cy ) ;
    //- Now update the dialog
    MoveWindow( 0, 0, cx, cy );
}

// VentShaftListDlg ��Ϣ��������

void VentShaftListDlg::OnBnClickedVentInShaftUpdateBtn()
{
    // �������ѡ���һ��
    updateLastSelect();

    // ���
    clearAll();

    // ��ʼ��list
    initTunnelList();
}

void VentShaftListDlg::OnLvnItemchangedList1( NMHDR* pNMHDR, LRESULT* pResult )
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
            // ��������
            UpdateData( TRUE );
            writePropertyData( m_objIds[m_lastPos] );

            // �ָ���һ��ѡ��ͼԪ����ɫ
            ArxEntityHelper::SetEntityColor( m_objIds[m_lastPos], m_colors[m_lastPos] );
        }

        // ��ȡ��ǰ���ͼԪ������
        readPropertyData( m_objIds[row] );
        UpdateData( FALSE ); // �������ݵ�����

        // ���Ų��û���ɫ������ʾ
        ArxEntityHelper::ZoomToEntity( m_objIds[row] );
        ArxEntityHelper::SetEntityColor( m_objIds[row], 2 );

        m_lastPos = row;

        acDocManager->unlockDocument( curDoc() );
    }
    /*else
    {
    	MessageBox(_T("lastpos =-1"));
    	m_propertyDataList.EnableWindow(FALSE);
    	m_propertyDataList.ShowWindow(SW_HIDE);
    }*/

    *pResult = 0;
}

BOOL VentShaftListDlg::OnInitDialog()
{
    CAcUiDialog::OnInitDialog();

    // ����listctrl�ı���ͷ
    m_list.SetExtendedStyle( m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
    CRect rect;
    m_list.GetClientRect( &rect );
    double c = 0.3;
    m_list.InsertColumn( 0, _T( "�羮���" ), LVCFMT_LEFT, rect.Width()*c );
    m_list.InsertColumn( 1, _T( "����/�ط�" ), LVCFMT_LEFT, rect.Width() * ( 1 - c ) );

    // ���
    clearAll();

    initTunnelList();

    //MessageBox(_T("��ʼ�����"));

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void VentShaftListDlg::initTunnelList()
{
    // ������ǰ�ĵ�
    acDocManager->lockDocument( curDoc() );

    AcDbObjectIdArray shafts;
    int pos = FindShafts( shafts );

    // ������ǰ�ĵ�
    acDocManager->unlockDocument( curDoc() );

    if( shafts.isEmpty() )
    {
        MessageBox( _T( "ͨ��ϵͳ��û�з羮!!!" ) );
        return;
    }

    // ������ǰ�ĵ�
    acDocManager->lockDocument( curDoc() );

    m_objIds.append( shafts );
    ArxEntityHelper::GetEntitiesColor( m_objIds, m_colors ); // ��¼��ɫ

    // ������ǰ�ĵ�
    acDocManager->unlockDocument( curDoc() );

    int len = shafts.length();
    for ( int i = 0; i < len; i++ )
    {
        // ���
        CString num;
        num.Format( _T( "%d" ), i + 1 );
        m_list.InsertItem( i, num );
        m_list.SetItemData( i, i );

        m_list.SetItemText( i, 0, num );
    }
    for( int i = 0; i < pos; i++ )	m_list.SetItemText( i, 1, _T( "���羮" ) );
    for( int i = pos; i < len; i++ ) m_list.SetItemText( i, 1, _T( "�ط羮" ) );

    // ѡ�е�1��
    if( len > 0 )
    {
        m_list.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );
        m_listFinishInitial = true;
    }
}

void VentShaftListDlg::clearAll()
{
    // ������ǰ�ĵ�
    acDocManager->lockDocument( curDoc() );

    // �ָ���ɫ
    ArxEntityHelper::SetEntitiesColor2( m_objIds, m_colors );

    acDocManager->unlockDocument( curDoc() );

    m_list.DeleteAllItems();
    m_objIds.removeAll();
    m_colors.removeAll();
    m_lastPos = -1;
    m_listFinishInitial = false;
}

void VentShaftListDlg::OnClosing()
{
    // �������ѡ���һ��
    updateLastSelect();

    // ���
    clearAll();
}

void VentShaftListDlg::OnBnClickedVentShaftParamBtn()
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
        //MessageBox(_T("���þ�Ͳ����"));
        // ������ǰ�ĵ�
        acDocManager->lockDocument( curDoc() );

        ShaftExtraPropertyDlg dlg;
        dlg.setRelatedShaft( m_objIds[row] );
        dlg.DoModal();

        acDocManager->unlockDocument( curDoc() );
    }
}

void VentShaftListDlg::readPropertyData( const AcDbObjectId& objId )
{
    // ��ȡ�������������
    DataHelper::GetPropertyData( objId, _T( "����" ), m_name );
    DataHelper::GetPropertyData( objId, _T( "�������" ), m_area );
    DataHelper::GetPropertyData( objId, _T( "����" ), m_velocity );
    DataHelper::GetPropertyData( objId, _T( "�¶�" ), m_temp );
}

void VentShaftListDlg::writePropertyData( const AcDbObjectId& objId )
{
    // �����������������
    DataHelper::SetPropertyData( objId, _T( "����" ), m_name );
    DataHelper::SetPropertyData( objId, _T( "�������" ), m_area );
    DataHelper::SetPropertyData( objId, _T( "����" ), m_velocity );
    DataHelper::SetPropertyData( objId, _T( "�¶�" ), m_temp );
}

void VentShaftListDlg::updateLastSelect()
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

        // ������ǰ�ĵ�
        acDocManager->lockDocument( curDoc() );
        // ��������
        writePropertyData( m_objIds[row] );

        acDocManager->unlockDocument( curDoc() );
    }
}