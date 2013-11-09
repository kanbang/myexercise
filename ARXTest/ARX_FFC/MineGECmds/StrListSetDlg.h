#pragma once

#include "afxwin.h"
#include "resource.h"

class StrListSetDlg : public CDialog
{
	DECLARE_DYNAMIC(StrListSetDlg)

public:
	StrListSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~StrListSetDlg();

// �Ի�������
	enum { IDD = IDD_STR_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStrAdd();
	afx_msg void OnBnClickedStrDel();
	afx_msg void OnBnClickedOk();

public:
	bool isStrExistInListBox(const CString& str);
	CListBox m_strListBox;

	AcStringArray m_strList;  // ����/���
};
