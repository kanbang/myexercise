#pragma once

#include "acui.h"
#include "resource.h"

class WDCI1Dlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(WDCI1Dlg)

public:
	WDCI1Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~WDCI1Dlg();

// �Ի�������
	enum { IDD = IDD_WDCI1_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CAcUiPickButton m_pickBtn;
	double m_xPos;
	double m_yPos;
	double m_zPos;
	double m_delta_h2;
	double m_smax;

	void readPropertyData();
	void writePropertyData();
	AcDbObjectId createObject();

public:
	AcDbObjectId m_objId;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedWdci1PtBtn();
};
