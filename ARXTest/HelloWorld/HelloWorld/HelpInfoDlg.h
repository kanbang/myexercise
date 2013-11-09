#pragma once

#include "resource.h"

class HelpInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(HelpInfoDlg)

public:
	HelpInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~HelpInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	void DrawHelpInfo(CDC* pDC);
	void DrawTitle1(CDC* pDC);
	void DrawTitle2(CDC* pDC);
	void DrawTitle3(CDC* pDC);
	void DrawTitle4(CDC* pDC);
};
