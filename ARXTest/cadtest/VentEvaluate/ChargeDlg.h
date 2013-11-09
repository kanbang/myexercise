#pragma once

#include "Resource.h"

class ChargeDlg : public CDialog
{
	DECLARE_DYNAMIC(ChargeDlg)

public:
	ChargeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ChargeDlg();

// 对话框数据
	enum { IDD = IDD_CHARGE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	AcDbObjectId m_objId;
	double m_h2;

	// 读写充电室数据
	void readPropertyData();
	void writePropertyData();
	
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
