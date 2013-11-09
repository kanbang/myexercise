// hildDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChildDlg.h"


// ChildDlg 对话框

IMPLEMENT_DYNAMIC( ChildDlg, CDialog )

ChildDlg::ChildDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( ChildDlg::IDD, pParent )
{

}

ChildDlg::~ChildDlg()
{
}

void ChildDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
}


BEGIN_MESSAGE_MAP( ChildDlg, CDialog )
    ON_CBN_SELCHANGE( IDC_COMBO1, &ChildDlg::OnCbnSelchangeCombo1 )
END_MESSAGE_MAP()


// ChildDlg 消息处理程序

void ChildDlg::OnCbnSelchangeCombo1()
{
    // TODO: 在此添加控件通知处理程序代码
}
