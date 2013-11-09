#pragma once

#include "resource.h"
#include "afxcmn.h"

class InstrumentDlg : public CDialog
{
	DECLARE_DYNAMIC(InstrumentDlg)

public:
	InstrumentDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~InstrumentDlg();

// 对话框数据
	enum { IDD = IDD_INSTRUMENT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_lastPos;
	AcDbObjectIdArray m_objIds;
	CListCtrl m_list;

	//CString m_name;
	CString m_type;
	CString m_num;
	CString m_factory;
	CString m_institute;
	BOOL m_ok;

	void initList();
	void updateLastSelect();
	void clearAll();
	void clearAllData();
	void readPropertyData(const AcDbObjectId& objId);
	void writePropertyData(const AcDbObjectId& objId);


	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnLvnItemchangedInstrumentList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedInstrumentAddBtn();
	afx_msg void OnBnClickedInstrumentDelBtn();
};
