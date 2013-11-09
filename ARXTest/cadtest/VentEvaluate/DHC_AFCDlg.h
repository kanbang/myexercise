#pragma once

#include "Resource.h"

class DHC_AFCDlg : public CDialog
{
	DECLARE_DYNAMIC(DHC_AFCDlg)

public:
	DHC_AFCDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DHC_AFCDlg();

// �Ի�������
	enum { IDD = IDD_DHC_AIR_FLOW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	AcDbObjectId m_objId;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDhcAfcPowderBtn();
	afx_msg void OnBnClickedDhcAfcChargeBtn();
	afx_msg void OnBnClickedDhcAfcMachineBtn();
};
