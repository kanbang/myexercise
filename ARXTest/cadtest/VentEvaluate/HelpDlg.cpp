#include "stdafx.h"
#include "HelpDlg.h"

IMPLEMENT_DYNAMIC( HelpDlg, PropertyData_DockBarChildDlg )

HelpDlg::HelpDlg( CWnd* pParent, HINSTANCE hInstance )
    : PropertyData_DockBarChildDlg( HelpDlg::IDD, pParent, hInstance )
{

}

HelpDlg::~HelpDlg()
{
}

void HelpDlg::DoDataExchange( CDataExchange* pDX )
{
    PropertyData_DockBarChildDlg::DoDataExchange( pDX );
}


BEGIN_MESSAGE_MAP( HelpDlg, PropertyData_DockBarChildDlg )
END_MESSAGE_MAP()


// HelpDlg 消息处理程序
