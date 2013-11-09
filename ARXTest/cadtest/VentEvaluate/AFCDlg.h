#pragma once

#include "Resource.h"

enum AFC_TYPE
{
	AT_WS = 0, 
	AT_TWS = 1,
	AT_DHC = 2,
	AT_OT  = 3
};

class AFCDlg : public CDialog
{
	DECLARE_DYNAMIC(AFCDlg)

public:
	AFCDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AFCDlg();

// �Ի�������
	enum { IDD = IDD_AFC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	AcDbObjectId m_objId;
	AFC_TYPE afc_type;
	
	/* �زɹ�������Ҫ�Ĳ��� */
	bool m_inHighGas;  // �Ƿ�λ�ڸ���˹����
	bool m_lowGasMine; // �Ƿ����˹��

	/* ������������Ҫ�Ĳ��� */
	bool m_isDiesel;  // �Ƿ��ֳ���

	void readPropertyData();
	void writePropertyData();

private:
	double userAllocateQ;

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedAfcBtn();
	afx_msg void OnBnClickedAfcUserAllocateQCheck();
protected:
	virtual void OnCancel();
};
