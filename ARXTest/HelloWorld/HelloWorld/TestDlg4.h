#pragma once

#include "resource.h"
#include "afxcmn.h"

class TestDlg4 : public CDialog
{
	DECLARE_DYNAMIC(TestDlg4)

public:
	TestDlg4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TestDlg4();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
};
