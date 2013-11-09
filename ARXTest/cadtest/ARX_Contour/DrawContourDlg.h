#pragma once

#include "Resource.h"

class DrawContourDlg : public CDialog
{
	DECLARE_DYNAMIC(DrawContourDlg)

public:
	DrawContourDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DrawContourDlg();

// �Ի�������
	enum { IDD = IDD_DRAW_CONTOUR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	int m_gct;
	double m_dz;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
