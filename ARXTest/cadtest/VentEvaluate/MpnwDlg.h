#pragma once

#include "Resource.h"

class MpnwDlg : public CDialog
{
	DECLARE_DYNAMIC(MpnwDlg)

public:
	MpnwDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MpnwDlg();

// �Ի�������
	enum { IDD = IDD_MPNW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	AcDbObjectId m_objId;
	void readPropertyData();
	void writePropertyData();

	int maxPeopleNum;
	double maxWeight;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
