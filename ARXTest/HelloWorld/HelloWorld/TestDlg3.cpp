// TestDlg3.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestDlg3.h"


// TestDlg3 �Ի���

IMPLEMENT_DYNAMIC( TestDlg3, CDialog )

TestDlg3::TestDlg3( CWnd* pParent /*=NULL*/ )
    : CDialog( TestDlg3::IDD, pParent )
{

}

TestDlg3::~TestDlg3()
{
}

void TestDlg3::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_PROGRESS1, m_prc );
}


BEGIN_MESSAGE_MAP( TestDlg3, CDialog )
END_MESSAGE_MAP()


BOOL TestDlg3::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_prc.SetRange( 1, 100 );
    m_prc.SetPos( 100 );

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}
