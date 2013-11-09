#pragma once

#include "PropertyData_DockBarChildDlg.h"

class GasPipeDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(GasPipeDockBarChildDlg)

public:
	GasPipeDockBarChildDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GasPipeDockBarChildDlg();

// �Ի�������
	enum { IDD = IDD_FFC_GAS_PIPE_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	void readPropertyData();
	void writePropertyData();

public:
	CString m_q;
	CString m_ch4;
	CString m_o2;
	virtual BOOL OnInitDialog();
};
