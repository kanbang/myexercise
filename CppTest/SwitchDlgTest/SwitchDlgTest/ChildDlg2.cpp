// ChildDlg2.cpp : 实现文件
//

#include "stdafx.h"
#include "SwitchDlgTest.h"
#include "ChildDlg2.h"


// ChildDlg2 对话框

IMPLEMENT_DYNAMIC( ChildDlg2, CDialog )

ChildDlg2::ChildDlg2( CWnd* pParent /*=NULL*/ )
    : CDialog( ChildDlg2::IDD, pParent )
{

}

ChildDlg2::~ChildDlg2()
{
}

void ChildDlg2::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
}


BEGIN_MESSAGE_MAP( ChildDlg2, CDialog )
END_MESSAGE_MAP()


// ChildDlg2 消息处理程序
