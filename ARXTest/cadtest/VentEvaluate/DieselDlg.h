#pragma once

#include "Resource.h"

class DieselDlg : public CDialog
{
	DECLARE_DYNAMIC(DieselDlg)

public:
	DieselDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DieselDlg();

// 对话框数据
	enum { IDD = IDD_DIESEL_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	AcDbObjectId m_objId;
	void readPropertyData();
	void writePropertyData();

private:
	int m_num;
	double m_power;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
