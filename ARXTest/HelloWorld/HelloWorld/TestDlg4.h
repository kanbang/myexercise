#pragma once

#include "resource.h"
#include "afxcmn.h"

class TestDlg4 : public CDialog
{
	DECLARE_DYNAMIC(TestDlg4)

public:
	TestDlg4(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TestDlg4();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
};
