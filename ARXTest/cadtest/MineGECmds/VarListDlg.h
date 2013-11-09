#pragma once

#include "resource.h"
#include "afxwin.h"

class VarListDlg : public CDialog
{
	DECLARE_DYNAMIC(VarListDlg)

public:
	VarListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~VarListDlg();

// �Ի�������
	enum { IDD = IDD_VAR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSetVar();
	afx_msg void OnBnClickedAddVar();
	afx_msg void OnBnClickedDelVar();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	virtual BOOL OnInitDialog();

private:
	void fillVarList();  // ����б�
	CString getSelVarName();
	bool isFieldExistInListBox(const CString& varName);
	int m_lt;
	CComboBox m_varList;
};
