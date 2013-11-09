#pragma once

#include "acui.h"
#include "resource.h"

class GPPDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(GPPDlg)

public:
	GPPDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GPPDlg();

// �Ի�������
	enum { IDD = IDD_GPP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CAcUiPickButton m_pickBtn;
	double m_xPos;
	double m_yPos;
	double m_zPos;
	double m_pressure;
	double m_level;
	double m_height;

	void readPropertyData();
	void writePropertyData();
	AcDbObjectId createObject();

public:
	AcDbObjectId m_objId;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedGppPtBtn();
};
