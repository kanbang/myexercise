#pragma once

#include "PropertyData_DockBarChildDlg.h"

class HelpDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(HelpDlg)

public:
	HelpDlg(CWnd *pParent =NULL, HINSTANCE hInstance =NULL);   // ��׼���캯��
	virtual ~HelpDlg();

// �Ի�������
	enum { IDD = IDD_HELP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
