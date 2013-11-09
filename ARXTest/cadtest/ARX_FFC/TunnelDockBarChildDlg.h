#pragma once

#include "PropertyData_DockBarChildDlg.h"

class TunnelDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(TunnelDockBarChildDlg)

public:
	TunnelDockBarChildDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TunnelDockBarChildDlg();

// �Ի�������
	enum { IDD = IDD_FFC_TUNNEL_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	void readPropertyData();
	void writePropertyData();

public:
	CString m_area;
	CString m_q;
	CString m_ch4;
	CString m_o2;

	virtual BOOL OnInitDialog();
};
