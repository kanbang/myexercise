#pragma once

#include "resource.h"

class MCDlg : public CDialog
{
	DECLARE_DYNAMIC(MCDlg)

public:
	MCDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MCDlg();

// 对话框数据
	enum { IDD = IDD_MC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	AcDbObjectId m_objId;
	void readPropertyData();
	void writePropertyData();

	double length;                // 工作面长度
	double max_kdj, min_kdj;	  // 最大控顶距和最小控顶距
	double height;                // 实际采高
	double reasonalVelocity;      // 适宜风速
	double k_height;              // 采高调整系数
	double k_length;              // 长度调整系数
	double k_temperature;         // 温度调整系数

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
