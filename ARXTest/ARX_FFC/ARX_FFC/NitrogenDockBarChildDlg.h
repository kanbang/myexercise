#pragma once

#include "PropertyData_DockBarChildDlg.h"

class NitrogenDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(NitrogenDockBarChildDlg)

public:
	NitrogenDockBarChildDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~NitrogenDockBarChildDlg();

// �Ի�������
	enum { IDD = IDD_FFC_NITROGEN_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	void readPropertyData();
	void writePropertyData();

public:
	CString m_n2;
	virtual BOOL OnInitDialog();
};
