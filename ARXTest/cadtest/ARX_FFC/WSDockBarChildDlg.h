#pragma once

#include "PropertyData_DockBarChildDlg.h"

class WSDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(WSDockBarChildDlg)

public:
	WSDockBarChildDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~WSDockBarChildDlg();

// 对话框数据
	enum { IDD = IDD_FFC_WS_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	void readPropertyData();
	void writePropertyData();

public:
	CString m_ch4;
	virtual BOOL OnInitDialog();
};
