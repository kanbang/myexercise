#pragma once

#include "Resource.h"

class ChargeDlg : public CDialog
{
	DECLARE_DYNAMIC(ChargeDlg)

public:
	ChargeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ChargeDlg();

// �Ի�������
	enum { IDD = IDD_CHARGE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	AcDbObjectId m_objId;
	double m_h2;

	// ��д���������
	void readPropertyData();
	void writePropertyData();
	
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
