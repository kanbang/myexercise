#include "stdafx.h"
#include "WSDockBarChildDlg.h"

#include "../MineGE/HelperClass.h"

IMPLEMENT_DYNAMIC( WSDockBarChildDlg, PropertyData_DockBarChildDlg )

WSDockBarChildDlg::WSDockBarChildDlg( CWnd* pParent /*=NULL*/ )
    : PropertyData_DockBarChildDlg( WSDockBarChildDlg::IDD, pParent )
    , m_ch4( _T( "" ) )
{

}

WSDockBarChildDlg::~WSDockBarChildDlg()
{
}

void WSDockBarChildDlg::DoDataExchange( CDataExchange* pDX )
{
    PropertyData_DockBarChildDlg::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_FFC_WS_Q, m_ch4 );
}


BEGIN_MESSAGE_MAP( WSDockBarChildDlg, PropertyData_DockBarChildDlg )
END_MESSAGE_MAP()


// WSDockBarChildDlg 消息处理程序

BOOL WSDockBarChildDlg::OnInitDialog()
{
    PropertyData_DockBarChildDlg::OnInitDialog();

    return TRUE;
}

void WSDockBarChildDlg::readPropertyData()
{
    DataHelper::GetPropertyData( m_objId, _T( "工作面瓦斯涌出量" ), m_ch4 );

    UpdateData( FALSE );

}

void WSDockBarChildDlg::writePropertyData()
{
    UpdateData( TRUE );

    DataHelper::SetPropertyData( m_objId, _T( "工作面瓦斯涌出量" ), m_ch4 );
}