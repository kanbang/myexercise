#pragma once

#include "resource.h"

class TecplotShowTypeDlg : public CDialog
{
	DECLARE_DYNAMIC(TecplotShowTypeDlg)

public:
	TecplotShowTypeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TecplotShowTypeDlg();

// 对话框数据
	enum { IDD = IDD_TECPLOT_SHOW_TYPE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int m_tst;
	afx_msg void OnBnClickedOk();
};
