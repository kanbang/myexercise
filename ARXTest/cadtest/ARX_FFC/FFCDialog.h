#pragma once

#include "resource.h"

class FFCDialog : public CDialog
{
	DECLARE_DYNAMIC(FFCDialog)

public:
	FFCDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FFCDialog();

// 对话框数据
	enum { IDD = IDD_FFC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFfcSolveBtn();
	afx_msg void OnBnClickedFfcDisplayBtn();
	afx_msg void OnBnClickedFfcKillBtn();
	
	afx_msg LRESULT OnAppendLog(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnClearLog(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnCloseThread(WPARAM wParam,LPARAM lParam);

private:
	int AppendToLog(CString str, COLORREF color);
	int AppendToLogAndScroll(CString str, COLORREF color);
	int GetNumVisibleLines(CRichEditCtrl* pCtrl);
	bool IsRunning() const;
	void SolveFFC();

	CRichEditCtrl m_ctrlLog;
};
