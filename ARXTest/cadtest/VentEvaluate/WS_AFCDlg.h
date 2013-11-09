#pragma once

#include "resource.h"

class WS_AFCDlg : public CDialog
{
	DECLARE_DYNAMIC(WS_AFCDlg)

public:
	WS_AFCDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~WS_AFCDlg();

// 对话框数据
	enum { IDD = IDD_WS_AIR_FLOW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	AcDbObjectId m_objId;
	bool m_inHighGas;  // 是否位于高瓦斯区域
	bool m_lowGasMine; // 是否低瓦斯矿井

	afx_msg void OnBnClickedAfcMcBtn();
	afx_msg void OnBnClickedAfcGasBtn();
	afx_msg void OnBnClickedAfcMpnwBtn();
	virtual BOOL OnInitDialog();	
};
