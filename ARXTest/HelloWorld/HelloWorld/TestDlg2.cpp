// TestDlg2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestDlg2.h"


// TestDlg2 �Ի���

IMPLEMENT_DYNAMIC( TestDlg2, CDialog )

TestDlg2::TestDlg2( CWnd* pParent /*=NULL*/ )
    : CDialog( TestDlg2::IDD, pParent )
{

}

TestDlg2::~TestDlg2()
{
}

void TestDlg2::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_PROGRESS1, m_prc );
}


BEGIN_MESSAGE_MAP( TestDlg2, CDialog )
END_MESSAGE_MAP()


// TestDlg2 ��Ϣ�������

BOOL TestDlg2::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_prc.SetRange( 1, 100 );
    m_prc.SetPos( 50 );

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}
