#pragma once


#include "resource.h"
#include "afxcmn.h"

class TestDlg2 : public CDialog
{
	DECLARE_DYNAMIC(TestDlg2)

public:
	TestDlg2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TestDlg2();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CProgressCtrl m_prc;
};
