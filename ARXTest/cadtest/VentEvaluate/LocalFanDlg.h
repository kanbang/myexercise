#pragma once

#include "Resource.h"

class LocalFanDlg : public CDialog
{
	DECLARE_DYNAMIC(LocalFanDlg)

public:
	LocalFanDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LocalFanDlg();

// �Ի�������
	enum { IDD = IDD_LOCAL_FAN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	AcDbObjectId m_objId;
	void readPropertyData();
	void writePropertyData();

private:
	double fanNum;
	double q_fan;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
