#pragma once

#include "PropertyData_DockBarChildDlg.h"

class ObturationDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(ObturationDockBarChildDlg)

public:
	ObturationDockBarChildDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ObturationDockBarChildDlg();

// �Ի�������
	enum { IDD = IDD_FFC_OBTURATION_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	void readPropertyData();
	void writePropertyData();

public:
	CString m_air_leak;
	virtual BOOL OnInitDialog();
};
