#pragma once

#include "PropertyData_DockBarChildDlg.h"

class SourceAirLeakDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(SourceAirLeakDockBarChildDlg)

public:
	SourceAirLeakDockBarChildDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SourceAirLeakDockBarChildDlg();

// 对话框数据
	enum { IDD = IDD_FFC_SOURCE_AIR_LEAK_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	void readPropertyData();
	void writePropertyData();

public:
	CString m_q;
	virtual BOOL OnInitDialog();
};
