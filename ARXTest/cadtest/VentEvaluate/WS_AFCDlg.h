#pragma once

#include "resource.h"

class WS_AFCDlg : public CDialog
{
	DECLARE_DYNAMIC(WS_AFCDlg)

public:
	WS_AFCDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~WS_AFCDlg();

// �Ի�������
	enum { IDD = IDD_WS_AIR_FLOW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	AcDbObjectId m_objId;
	bool m_inHighGas;  // �Ƿ�λ�ڸ���˹����
	bool m_lowGasMine; // �Ƿ����˹��

	afx_msg void OnBnClickedAfcMcBtn();
	afx_msg void OnBnClickedAfcGasBtn();
	afx_msg void OnBnClickedAfcMpnwBtn();
	virtual BOOL OnInitDialog();	
};
