#pragma once

#include "acui.h"
#include "resource.h"

class CDHDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CDHDlg)

public:
	CDHDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDHDlg();

// �Ի�������
	enum { IDD = IDD_CDH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CAcUiPickButton m_pickBtn;
	double m_xPos;
	double m_yPos;
	double m_zPos;
	int m_num;
	double m_h1;
	double m_h2;
	double m_thick;

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
