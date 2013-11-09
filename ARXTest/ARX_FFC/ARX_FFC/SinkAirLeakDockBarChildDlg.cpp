#include "stdafx.h"
#include "SinkAirLeakDockBarChildDlg.h"

#include "../MineGE/HelperClass.h"

IMPLEMENT_DYNAMIC( SinkAirLeakDockBarChildDlg, PropertyData_DockBarChildDlg )

SinkAirLeakDockBarChildDlg::SinkAirLeakDockBarChildDlg( CWnd* pParent /*=NULL*/ )
    : PropertyData_DockBarChildDlg( SinkAirLeakDockBarChildDlg::IDD, pParent )
    , m_q( _T( "" ) )
{

}

SinkAirLeakDockBarChildDlg::~SinkAirLeakDockBarChildDlg()
{
}

void SinkAirLeakDockBarChildDlg::DoDataExchange( CDataExchange* pDX )
{
    PropertyData_DockBarChildDlg::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_FFC_SINK_AIR_LEAK_Q, m_q );
}


BEGIN_MESSAGE_MAP( SinkAirLeakDockBarChildDlg, PropertyData_DockBarChildDlg )
END_MESSAGE_MAP()


// SinkAirLeakDockBarChildDlg 消息处理程序

BOOL SinkAirLeakDockBarChildDlg::OnInitDialog()
{
    PropertyData_DockBarChildDlg::OnInitDialog();

    return TRUE;
}

void SinkAirLeakDockBarChildDlg::readPropertyData()
{
    DataHelper::GetPropertyData( m_objId, _T( "漏风量" ), m_q );

    UpdateData( FALSE );
}

void SinkAirLeakDockBarChildDlg::writePropertyData()
{
    UpdateData( TRUE );

    DataHelper::SetPropertyData( m_objId, _T( "漏风量" ), m_q );
}