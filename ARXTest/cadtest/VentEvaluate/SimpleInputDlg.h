#pragma once

#include "resource.h"

// ����Ի���
class SimpleInputDlg : public CDialog
{
	DECLARE_DYNAMIC(SimpleInputDlg)

public:
	SimpleInputDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SimpleInputDlg();

	// �Ի�������
	enum { IDD = IDD_SIMPLE_INPUT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_str;     // �����
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};