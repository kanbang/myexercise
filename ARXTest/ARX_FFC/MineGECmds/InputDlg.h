#pragma once

#include "resource.h"

// ����Ի���
class InputDlg : public CDialog
{
	DECLARE_DYNAMIC(InputDlg)

public:
	InputDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~InputDlg();

	// �Ի�������
	enum { IDD = IDD_INPUT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_str;     // �����
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};