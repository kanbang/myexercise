#pragma once

#include "Resource.h"

class TWS_AFCDlg : public CDialog
{
	DECLARE_DYNAMIC(TWS_AFCDlg)

public:
	TWS_AFCDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TWS_AFCDlg();

// �Ի�������
	enum { IDD = IDD_TWS_AIR_FLOW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	AcDbObjectId m_objId;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTwsAfcGasBtn();
	afx_msg void OnBnClickedTwsAfcLocalFanBtn();
	afx_msg void OnBnClickedTwsAfcMpnwBtn();
};
