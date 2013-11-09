#pragma once

#include "resource.h"

class AdvanceDlg : public CDialog
{
	DECLARE_DYNAMIC(AdvanceDlg)

public:
	AdvanceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AdvanceDlg();

// 对话框数据
	enum { IDD = IDD_ADVANCE_WS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dist;
	afx_msg void OnBnClickedOk();
};
