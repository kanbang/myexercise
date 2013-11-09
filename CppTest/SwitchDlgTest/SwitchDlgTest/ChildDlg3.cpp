// ChildDlg3.cpp : 实现文件
//

#include "stdafx.h"
#include "SwitchDlgTest.h"
#include "ChildDlg3.h"


// ChildDlg3 对话框

IMPLEMENT_DYNAMIC( ChildDlg3, CDialog )

ChildDlg3::ChildDlg3( CWnd* pParent /*=NULL*/ )
    : CDialog( ChildDlg3::IDD, pParent )
{

}

ChildDlg3::~ChildDlg3()
{
}

void ChildDlg3::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
}


BEGIN_MESSAGE_MAP( ChildDlg3, CDialog )
END_MESSAGE_MAP()


// ChildDlg3 消息处理程序
