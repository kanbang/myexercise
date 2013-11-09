#pragma once

#include "Resource.h"

class PowderDlg : public CDialog
{
	DECLARE_DYNAMIC(PowderDlg)

public:
	PowderDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PowderDlg();

// �Ի�������
	enum { IDD = IDD_POWDER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	AcDbObjectId m_objId;
	double m_volume;

private:
	void readPropertyData();
	void writePropertyData();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
