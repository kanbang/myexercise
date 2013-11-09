#include "stdafx.h"
#include "DrawContourDlg.h"

IMPLEMENT_DYNAMIC( DrawContourDlg, CDialog )

DrawContourDlg::DrawContourDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( DrawContourDlg::IDD, pParent )
    , m_gct( 0 )
    , m_dz( 0 )
{

}

DrawContourDlg::~DrawContourDlg()
{
}

void DrawContourDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Radio( pDX, IDC_GFP_RADIO, m_gct );
    DDX_Text( pDX, IDC_CONTOUR_DZ_EDIT, m_dz );
}

BEGIN_MESSAGE_MAP( DrawContourDlg, CDialog )
    ON_BN_CLICKED( IDOK, &DrawContourDlg::OnBnClickedOk )
END_MESSAGE_MAP()

BOOL DrawContourDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    return TRUE;
}

void DrawContourDlg::OnBnClickedOk()
{
    if( !UpdateData( TRUE ) ) return;

    if( abs( m_dz ) < 1.0e-4 )
    {
        MessageBox( _T( "等量差不能等于0, 误差精度为:0.00001" ) );
    }
    else
    {
        OnOK();
    }
}
