#pragma once

#include "resource.h"

class ContourParamDlg : public CDialog
{
	DECLARE_DYNAMIC(ContourParamDlg)

public:
	ContourParamDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ContourParamDlg();

// �Ի�������
	enum { IDD = IDD_CONTOUR_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	double m_textHeight;
	BOOL m_bSmooth;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
