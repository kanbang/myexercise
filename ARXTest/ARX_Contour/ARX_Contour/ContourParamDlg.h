#pragma once

#include "resource.h"

class ContourParamDlg : public CDialog
{
	DECLARE_DYNAMIC(ContourParamDlg)

public:
	ContourParamDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ContourParamDlg();

// 对话框数据
	enum { IDD = IDD_CONTOUR_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	double m_textHeight;
	BOOL m_bSmooth;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
