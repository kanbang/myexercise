#include "stdafx.h"
#include "ObturationDockBarChildDlg.h"

#include "../MineGE/HelperClass.h"

IMPLEMENT_DYNAMIC( ObturationDockBarChildDlg, PropertyData_DockBarChildDlg )

ObturationDockBarChildDlg::ObturationDockBarChildDlg( CWnd* pParent /*=NULL*/ )
    : PropertyData_DockBarChildDlg( ObturationDockBarChildDlg::IDD, pParent )
    , m_air_leak( _T( "" ) )
{

}

ObturationDockBarChildDlg::~ObturationDockBarChildDlg()
{
}

void ObturationDockBarChildDlg::DoDataExchange( CDataExchange* pDX )
{
    PropertyData_DockBarChildDlg::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_FFC_OBTURATION_AIR_LEAK, m_air_leak );
    DDX_Text( pDX, IDC_FFC_OBTURATION_CH4, m_ch4 );
    DDX_Text( pDX, IDC_FFC_OBTURATION_O2, m_o2 );
}


BEGIN_MESSAGE_MAP( ObturationDockBarChildDlg, PropertyData_DockBarChildDlg )
END_MESSAGE_MAP()


BOOL ObturationDockBarChildDlg::OnInitDialog()
{
    PropertyData_DockBarChildDlg::OnInitDialog();

    return TRUE;
}

void ObturationDockBarChildDlg::readPropertyData()
{
    DataHelper::GetPropertyData( m_objId, _T( "漏风量" ), m_air_leak );
    DataHelper::GetPropertyData( m_objId, _T( "瓦斯浓度" ), m_ch4 );
    DataHelper::GetPropertyData( m_objId, _T( "氧气浓度" ), m_o2 );

    UpdateData( FALSE );
}

void ObturationDockBarChildDlg::writePropertyData()
{
    UpdateData( TRUE );

    DataHelper::SetPropertyData( m_objId, _T( "漏风量" ), m_air_leak );
    DataHelper::SetPropertyData( m_objId, _T( "瓦斯浓度" ), m_ch4 );
    DataHelper::SetPropertyData( m_objId, _T( "氧气浓度" ), m_o2 );
}