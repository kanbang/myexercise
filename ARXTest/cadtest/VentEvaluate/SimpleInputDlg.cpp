#include "stdafx.h"
#include "SimpleInputDlg.h"

IMPLEMENT_DYNAMIC( SimpleInputDlg, CDialog )

SimpleInputDlg::SimpleInputDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( SimpleInputDlg::IDD, pParent ), m_str( _T( "" ) )
{

}

SimpleInputDlg::~SimpleInputDlg()
{
}

void SimpleInputDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_INPUT_STR_EDIT, m_str );
}

BEGIN_MESSAGE_MAP( SimpleInputDlg, CDialog )
    ON_BN_CLICKED( IDOK, &SimpleInputDlg::OnBnClickedOk )
END_MESSAGE_MAP()

BOOL SimpleInputDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    GetDlgItem( IDC_INPUT_STR_EDIT )->SetFocus();

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void SimpleInputDlg::OnBnClickedOk()
{
    UpdateData( TRUE ); // ���¿ؼ�����

    m_str.Trim(); // ȥ���հ�

    if( m_str.GetLength() == 0 )
    {
        MessageBox( _T( "������������" ) );
    }
    else
    {
        OnOK();
    }
}
