#pragma once

#include "resource.h"
#include "afxwin.h"

class VNGGenDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(VNGGenDlg)

public:
	VNGGenDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~VNGGenDlg();

	enum { IDD = IDD_VNG_GEN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CString m_dbPath;

public:
	afx_msg void OnBnClickedPickBtn();
	afx_msg void OnBnClickedGraphParamBtn();
	afx_msg void OnBnClickedNodeParamBtn();
	afx_msg void OnBnClickedEdgeParamBtn();
	afx_msg void OnBnClickedDbPathSelectBtn();
	afx_msg void OnBnClickedOk();

	virtual BOOL OnInitDialog();
};
