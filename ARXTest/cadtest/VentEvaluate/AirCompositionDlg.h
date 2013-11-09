#pragma once

#include "resource.h"

class AirCompositionDlg : public CDialog
{
	DECLARE_DYNAMIC(AirCompositionDlg)

public:
	AirCompositionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AirCompositionDlg();

// �Ի�������
	enum { IDD = IDD_AIR_COMPOSITION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	AcDbObjectId m_objId;
	void readPropertyData();
	void writePropertyData();

private:
	double O2;
	double CO2;
	double CO;
	double NO;
	double SO2;
	double H2S;
	double NH3;

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
