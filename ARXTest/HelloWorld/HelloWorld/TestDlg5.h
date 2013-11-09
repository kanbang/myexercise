#pragma once

#include "ChartCtrl.h"
#include "ChartLineSerie.h"
#include "resource.h"

class TestDlg5 : public CDialog
{
	DECLARE_DYNAMIC(TestDlg5)

public:
	TestDlg5(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TestDlg5();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	void BuildXYValues(double Q_min, double Q_max, int n, double* XValues, double* YValues);
	double H(double Q);

	CChartCtrl m_ChartCtrl;
	double a0;
	double a1;
	double a2;
	double a3;
	double a4;
	double a5;
};
