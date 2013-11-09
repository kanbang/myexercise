#pragma once


#include "resource.h"
#include "afxcmn.h"

class TestDlg2 : public CDialog
{
	DECLARE_DYNAMIC(TestDlg2)

public:
	TestDlg2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TestDlg2();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CProgressCtrl m_prc;
};
