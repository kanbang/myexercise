#pragma once

#include "resource.h"

class AdvanceDlg : public CDialog
{
	DECLARE_DYNAMIC(AdvanceDlg)

public:
	AdvanceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AdvanceDlg();

// �Ի�������
	enum { IDD = IDD_ADVANCE_WS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_dist;
	afx_msg void OnBnClickedOk();
};
