#pragma once

#include "PropertyData_DockBarChildDlg.h"

class ObturationDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(ObturationDockBarChildDlg)

public:
	ObturationDockBarChildDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ObturationDockBarChildDlg();

// 对话框数据
	enum { IDD = IDD_FFC_OBTURATION_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	void readPropertyData();
	void writePropertyData();

public:
	CString m_air_leak;
	virtual BOOL OnInitDialog();
};
