#pragma once

#include "PropertyData_DockBarChildDlg.h"

class GasPipeDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(GasPipeDockBarChildDlg)

public:
	GasPipeDockBarChildDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GasPipeDockBarChildDlg();

// 对话框数据
	enum { IDD = IDD_FFC_GAS_PIPE_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	void readPropertyData();
	void writePropertyData();

public:
	CString m_q;
	CString m_ch4;
	CString m_o2;
	virtual BOOL OnInitDialog();
};
