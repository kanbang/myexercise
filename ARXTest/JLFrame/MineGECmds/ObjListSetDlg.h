#pragma once

#include "resource.h"
#include "afxwin.h"

class ObjListSetDlg : public CDialog
{
	DECLARE_DYNAMIC(ObjListSetDlg)

public:
	ObjListSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ObjListSetDlg();

// �Ի�������
	enum { IDD = IDD_OBJ_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStrAdd();
	afx_msg void OnBnClickedStrDel();
	afx_msg void OnBnClickedOk();
	
	bool isStrExistInListBox(const CString& str);
	CListBox m_strListBox;
	AcStringArray m_strList;  // ����/���
	virtual BOOL OnInitDialog();
};
