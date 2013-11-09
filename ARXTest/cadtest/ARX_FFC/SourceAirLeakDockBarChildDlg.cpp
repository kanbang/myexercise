#include "stdafx.h"
#include "SourceAirLeakDockBarChildDlg.h"

#include "../MineGE/HelperClass.h"

IMPLEMENT_DYNAMIC( SourceAirLeakDockBarChildDlg, PropertyData_DockBarChildDlg )

SourceAirLeakDockBarChildDlg::SourceAirLeakDockBarChildDlg( CWnd* pParent /*=NULL*/ )
    : PropertyData_DockBarChildDlg( SourceAirLeakDockBarChildDlg::IDD, pParent )
    , m_q( _T( "" ) )
{

}

SourceAirLeakDockBarChildDlg::~SourceAirLeakDockBarChildDlg()
{
}

void SourceAirLeakDockBarChildDlg::DoDataExchange( CDataExchange* pDX )
{
    PropertyData_DockBarChildDlg::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_FFC_SOURCE_AIR_LEAK_Q, m_q );
}


BEGIN_MESSAGE_MAP( SourceAirLeakDockBarChildDlg, PropertyData_DockBarChildDlg )
END_MESSAGE_MAP()


// SourceAirLeakDockBarChildDlg ��Ϣ�������

BOOL SourceAirLeakDockBarChildDlg::OnInitDialog()
{
    PropertyData_DockBarChildDlg::OnInitDialog();

    return TRUE;
}

void SourceAirLeakDockBarChildDlg::readPropertyData()
{
    DataHelper::GetPropertyData( m_objId, _T( "©����" ), m_q );

    UpdateData( FALSE );
}

void SourceAirLeakDockBarChildDlg::writePropertyData()
{
    UpdateData( TRUE );

    DataHelper::SetPropertyData( m_objId, _T( "©����" ), m_q );
}