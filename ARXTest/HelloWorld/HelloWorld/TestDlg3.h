#pragma once

#include "resource.h"
#include "afxcmn.h"

class TestDlg3 : public CDialog
{
	DECLARE_DYNAMIC(TestDlg3)

public:
	TestDlg3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TestDlg3();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_prc;
	virtual BOOL OnInitDialog();
};
