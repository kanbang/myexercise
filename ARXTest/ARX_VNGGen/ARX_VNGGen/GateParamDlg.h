#pragma once

#include "resource.h"
#include "VNGParam.h"

class GateParamDlg : public CDialog
{
	DECLARE_DYNAMIC(GateParamDlg)

public:
	GateParamDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GateParamDlg();

// 对话框数据
	enum { IDD = IDD_GATE_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	GateParam gp;

private:
	double m_radius;
	double m_length;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedGateDefBtn();
};
