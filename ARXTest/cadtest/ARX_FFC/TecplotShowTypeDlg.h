#pragma once

#include "resource.h"

class TecplotShowTypeDlg : public CDialog
{
	DECLARE_DYNAMIC(TecplotShowTypeDlg)

public:
	TecplotShowTypeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TecplotShowTypeDlg();

// �Ի�������
	enum { IDD = IDD_TECPLOT_SHOW_TYPE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	int m_tst;
	afx_msg void OnBnClickedOk();
};
