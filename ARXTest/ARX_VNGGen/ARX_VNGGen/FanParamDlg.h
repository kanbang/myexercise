#pragma once

#include "resource.h"
#include "VNGParam.h"

class FanParamDlg : public CDialog
{
	DECLARE_DYNAMIC(FanParamDlg)

public:
	FanParamDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FanParamDlg();

	enum { IDD = IDD_FAN_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	FanParam fp;

private:
	double m_radius;
	double m_distance;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedFanDefBtn();
};
