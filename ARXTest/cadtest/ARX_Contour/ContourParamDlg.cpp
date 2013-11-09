#include "stdafx.h"
#include "ContourParamDlg.h"

#include "ContourParamHelper.h"

IMPLEMENT_DYNAMIC( ContourParamDlg, CDialog )

ContourParamDlg::ContourParamDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( ContourParamDlg::IDD, pParent )
    , m_textHeight( 0 )
    , m_bSmooth( FALSE )
{

}

ContourParamDlg::~ContourParamDlg()
{
}

void ContourParamDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_CONTOUR_TEXT_HEIGHT, m_textHeight );
    DDX_Check( pDX, IDC_CONTOUR_SMOOTH, m_bSmooth );
}


BEGIN_MESSAGE_MAP( ContourParamDlg, CDialog )
    ON_BN_CLICKED( IDOK, &ContourParamDlg::OnBnClickedOk )
END_MESSAGE_MAP()

BOOL ContourParamDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    bool bSmooth;
    ContourParamHelper::ReadParams( bSmooth, m_textHeight );
    m_bSmooth = ( bSmooth ? 1 : 0 );

    UpdateData( FALSE );

    return TRUE;
}

void ContourParamDlg::OnBnClickedOk()
{
    if( UpdateData( TRUE ) )
    {
        ContourParamHelper::WriteParams( m_bSmooth != 0, m_textHeight );
        OnOK();
    }
}
