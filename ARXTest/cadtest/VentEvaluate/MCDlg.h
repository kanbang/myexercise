#pragma once

#include "resource.h"

class MCDlg : public CDialog
{
	DECLARE_DYNAMIC(MCDlg)

public:
	MCDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MCDlg();

// �Ի�������
	enum { IDD = IDD_MC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	AcDbObjectId m_objId;
	void readPropertyData();
	void writePropertyData();

	double length;                // �����泤��
	double max_kdj, min_kdj;	  // ���ض������С�ض���
	double height;                // ʵ�ʲɸ�
	double reasonalVelocity;      // ���˷���
	double k_height;              // �ɸߵ���ϵ��
	double k_length;              // ���ȵ���ϵ��
	double k_temperature;         // �¶ȵ���ϵ��

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
