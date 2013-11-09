#pragma once

#include "DockBarChildDlg.h"

class DataListDlg : public DockBarChildDlg
{
	DECLARE_DYNAMIC(DataListDlg)

public:
	DataListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DataListDlg();

// �Ի�������
	enum { IDD = IDD_DATA_LIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_typeList;
	AcStringArray m_types;

	CListCtrl m_geList;
	bool m_initFinished;

public:
	virtual BOOL OnInitDialog();
	virtual void OnClosing();
	afx_msg void OnBnClickedUpdateBtn();
	afx_msg void OnBnClickedHilightSelectBtn();
	afx_msg void OnLvnItemchangedDataList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickGeList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSelectBtn();
};
