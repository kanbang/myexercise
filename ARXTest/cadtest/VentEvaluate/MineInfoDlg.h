#pragma once

#include "Resource.h"
#include "afxwin.h"
#include "afxcmn.h"

// 煤矿信息设置对话框
class MineInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(MineInfoDlg)

public:
	MineInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MineInfoDlg();

// 对话框数据
	enum { IDD = IDD_MINE_INFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
	CString m_company;
	COleDateTime m_designDate;
	COleDateTime m_workDate;
	CString m_quality;
	CComboBox m_gasLevel;
	CComboBox m_vent;
	CSliderCtrl m_slider1;
	CSliderCtrl m_slider2;
	CString m_totalQ;

public:
	void readPropertyData();
	void writePropertyData();
	void initSlider(CSliderCtrl& sl, int minR, int maxR, int pos, long id);
	void updateCoeff(long id, int v);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
