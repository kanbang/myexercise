#include "stdafx.h"
#include "TecplotShowTypeDlg.h"

IMPLEMENT_DYNAMIC( TecplotShowTypeDlg, CDialog )

TecplotShowTypeDlg::TecplotShowTypeDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( TecplotShowTypeDlg::IDD, pParent ),
      m_tst( 0 )
{

}

TecplotShowTypeDlg::~TecplotShowTypeDlg()
{
}

void TecplotShowTypeDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Radio( pDX, IDC_TST_RADIO1, m_tst );
}

BEGIN_MESSAGE_MAP( TecplotShowTypeDlg, CDialog )
    ON_BN_CLICKED( IDOK, &TecplotShowTypeDlg::OnBnClickedOk )
END_MESSAGE_MAP()

void TecplotShowTypeDlg::OnBnClickedOk()
{
    UpdateData( TRUE );

    OnOK();
}
