#pragma once

#include "Resource.h"
#include "afxwin.h"

// ���ӻ�Ч������Ի���
class DrawManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(DrawManagerDlg)

public:
	DrawManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DrawManagerDlg();

// �Ի�������
	enum { IDD = IDD_DRAW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_typeCombo;
	CComboBox m_drawCombo;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCfgBtn();
	afx_msg void OnCbnSelchangeTypeCombo();
	afx_msg void OnBnClickedSetBtn();

private:
	void clearDrawComboContent();
	int findDrawIndex(const CString& geType, const AcStringArray& drawList);
	CString getSelType();
	CString getSelDraw();
};
