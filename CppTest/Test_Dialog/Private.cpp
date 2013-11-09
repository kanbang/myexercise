// Private.cpp : 实现文件
//

#include "stdafx.h"
#include "Test_Dialog.h"
#include "Private.h"


// CPrivate 对话框

IMPLEMENT_DYNAMIC( CPrivate, CDialog )

CPrivate::CPrivate( CWnd* pParent /*=NULL*/ )
    : CDialog( CPrivate::IDD, pParent )
    , m_teststr( _T( "" ) )
{

}

CPrivate::~CPrivate()
{
}

void CPrivate::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_STATIC12, m_teststr );
}


BEGIN_MESSAGE_MAP( CPrivate, CDialog )
    ON_BN_CLICKED( IDOK, &CPrivate::OnBnClickedOk )
END_MESSAGE_MAP()


// CPrivate 消息处理程序

void CPrivate::OnBnClickedOk()
{

    CTest_DialogDlg dlg;
    m_csabout = dlg.Get();
    MessageBox( m_csabout );

//	UpdateData(FALSE);
}
