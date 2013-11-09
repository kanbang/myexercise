#pragma once

#include "acui.h"
#include "resource.h"

class WGFPDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(WGFPDlg)

public:
	WGFPDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~WGFPDlg();

// �Ի�������
	enum { IDD = IDD_WGFP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CAcUiPickButton m_pickBtn;
	double m_xPos;
	double m_yPos;
	double m_zPos;
	double m_coal;

	double m_aq;
	double m_rq;
	double m_t;
	CString m_dt;

	void readPropertyData();
	void writePropertyData();
	AcDbObjectId createObject();

public:
	AcDbObjectId m_objId;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedGopPtBtn();
};
