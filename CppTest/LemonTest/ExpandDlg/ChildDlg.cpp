// hildDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChildDlg.h"


// ChildDlg �Ի���

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


// ChildDlg ��Ϣ�������

void ChildDlg::OnCbnSelchangeCombo1()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
}
