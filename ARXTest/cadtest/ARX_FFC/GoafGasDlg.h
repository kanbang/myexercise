#pragma once

#include "resource.h"

class GoafGasDlg : public CDialog
{
	DECLARE_DYNAMIC(GoafGasDlg)

public:
	GoafGasDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GoafGasDlg();

// �Ի�������
	enum { IDD = IDD_GOAF_GAS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_goaf_gas;
	double m_side_gas;
	double m_cut_gas;
	double m_top_gas;
	double m_bottom_gas;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
