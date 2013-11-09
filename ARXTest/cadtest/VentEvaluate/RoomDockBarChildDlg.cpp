#include "stdafx.h"
#include "RoomDockBarChildDlg.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

#include "AFCDlg.h"

IMPLEMENT_DYNAMIC( RoomDockBarChildDlg, PropertyData_DockBarChildDlg )

RoomDockBarChildDlg::RoomDockBarChildDlg( CWnd* pParent, HINSTANCE hInstance )
    : PropertyData_DockBarChildDlg( RoomDockBarChildDlg::IDD, pParent, hInstance )
    , m_area( _T( "" ) )
    , m_velocity( _T( "" ) )
{

}

RoomDockBarChildDlg::~RoomDockBarChildDlg()
{
}

void RoomDockBarChildDlg::DoDataExchange( CDataExchange* pDX )
{
    PropertyData_DockBarChildDlg::DoDataExchange( pDX );

    DDX_Text( pDX, IDC_ROOM_NAME_EDIT, m_name );
    DDX_Text( pDX, IDC_ROOM_AREA_EDIT, m_area );
    DDX_Text( pDX, IDC_ROOM_VELOCITY_EDIT, m_velocity );
    DDX_Text( pDX, IDC_ROOM_Q_EDIT, m_q );
    DDX_Text( pDX, IDC_ROOM_TEMP_EDIT, m_temp );
    DDX_Radio( pDX, IDC_ROOM_TEMP_MEASURE_RADIO1, m_cm );
}

BEGIN_MESSAGE_MAP( RoomDockBarChildDlg, PropertyData_DockBarChildDlg )
    ON_BN_CLICKED( IDC_ROOM_AFC_BTN, &RoomDockBarChildDlg::OnBnClickedRoomAfcBtn )
END_MESSAGE_MAP()

BOOL RoomDockBarChildDlg::OnInitDialog()
{
    PropertyData_DockBarChildDlg::OnInitDialog();

    GetDlgItem( IDC_ROOM_OTHER_PARAM_BTN )->EnableWindow( FALSE );

    return TRUE;
}

void RoomDockBarChildDlg::readPropertyData()
{
    // ����ͨ������
    DataHelper::GetPropertyData( m_objId, _T( "����" ), m_name );
    DataHelper::GetPropertyData( m_objId, _T( "�������" ), m_area );
    DataHelper::GetPropertyData( m_objId, _T( "����" ), m_velocity );
    DataHelper::GetPropertyData( m_objId, _T( "����" ), m_q );
    DataHelper::GetPropertyData( m_objId, _T( "�¶�" ), m_temp );
    readControlMeasure();

    UpdateData( FALSE );
}

void RoomDockBarChildDlg::writePropertyData()
{
    UpdateData( TRUE );

    DataHelper::SetPropertyData( m_objId, _T( "����" ), m_name );
    DataHelper::SetPropertyData( m_objId, _T( "����" ), m_q );
    DataHelper::SetPropertyData( m_objId, _T( "�������" ), m_area );
    DataHelper::SetPropertyData( m_objId, _T( "����" ), m_velocity );
    DataHelper::SetPropertyData( m_objId, _T( "�¶�" ), m_temp );
    writeControlMeasure();
}

void RoomDockBarChildDlg::readControlMeasure()
{
    CString v1;
    DataHelper::GetPropertyData( m_objId, _T( "���¿��ƴ�ʩ" ), v1 );
    int i1 = 0;
    ArxUtilHelper::StringToInt( v1, i1 );
    m_cm = i1 % 3; // 0-��,1-���ƴ�ʩ,2-ֹͣ��ҵ
}

void RoomDockBarChildDlg::writeControlMeasure()
{
    CString v1;
    v1.Format( _T( "%d" ), m_cm );
    DataHelper::SetPropertyData( m_objId, _T( "���¿��ƴ�ʩ" ), v1 );
}

void RoomDockBarChildDlg::OnBnClickedRoomAfcBtn()
{
    CAcModuleResourceOverride myResources;

    acDocManager->lockDocument( curDoc() );

    AFCDlg dlg;
    dlg.m_objId = m_objId;
    dlg.afc_type = AT_DHC;
    dlg.DoModal();

    acDocManager->unlockDocument( curDoc() );
}
