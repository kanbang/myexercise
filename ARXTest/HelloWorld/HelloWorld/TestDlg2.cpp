// TestDlg2.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDlg2.h"


// TestDlg2 对话框

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


// TestDlg2 消息处理程序

BOOL TestDlg2::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_prc.SetRange( 1, 100 );
    m_prc.SetPos( 50 );

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
