#pragma once

#include "resource.h"

class GambitParamDlg : public CDialog
{
	DECLARE_DYNAMIC(GambitParamDlg)

public:
	GambitParamDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GambitParamDlg();

// 对话框数据
	enum { IDD = IDD_FFC_GAMBIT_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_width;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
