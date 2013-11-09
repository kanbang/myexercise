#pragma once

#include "resource.h"

// 输入对话框
class SimpleInputDlg : public CDialog
{
	DECLARE_DYNAMIC(SimpleInputDlg)

public:
	SimpleInputDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SimpleInputDlg();

	// 对话框数据
	enum { IDD = IDD_SIMPLE_INPUT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_str;     // 输入框
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};