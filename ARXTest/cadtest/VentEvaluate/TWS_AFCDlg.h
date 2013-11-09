#pragma once

#include "Resource.h"

class TWS_AFCDlg : public CDialog
{
	DECLARE_DYNAMIC(TWS_AFCDlg)

public:
	TWS_AFCDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TWS_AFCDlg();

// 对话框数据
	enum { IDD = IDD_TWS_AIR_FLOW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	AcDbObjectId m_objId;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTwsAfcGasBtn();
	afx_msg void OnBnClickedTwsAfcLocalFanBtn();
	afx_msg void OnBnClickedTwsAfcMpnwBtn();
};
