#pragma once

#include "acui.h"
#include "resource.h"

class PPPDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(PPPDlg)

public:
	PPPDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PPPDlg();

// �Ի�������
	enum { IDD = IDD_PPP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CAcUiPickButton m_pickBtn;
	double m_xPos;
	double m_yPos;
	double m_zPos;
	double m_coal;
	double m_gas;
	double m_level;
	CString m_dt;

	void readPropertyData();
	void writePropertyData();
	AcDbObjectId createObject();

public:
	AcDbObjectId m_objId;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedPppPtBtn();
};
