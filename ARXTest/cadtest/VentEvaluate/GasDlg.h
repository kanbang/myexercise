#pragma once

#include "Resource.h"

enum GAS_TYPE
{
	GT_TUNNEL = 0,
	GT_WS     = 1,
	GT_TWS    = 2
};

class GasDlg : public CDialog
{
	DECLARE_DYNAMIC(GasDlg)

public:
	GasDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GasDlg();

// �Ի�������
	enum { IDD = IDD_GAS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	AcDbObjectId m_objId;
	GAS_TYPE gas_type;
	void readPropertyData();
	void writePropertyData();

private:
	double q_ch4;
	double k_ch4;
	double q_ch4_tail;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
