#pragma once

#include "acui.h"
#include "resource.h"

class PointForecastDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(PointForecastDlg)

public:
	PointForecastDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PointForecastDlg();

// �Ի�������
	enum { IDD = IDD_POINT_FORECAST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CListCtrl m_dataList;
	CAcUiPickButton m_pickBtn;
	double m_xPos;
	double m_yPos;
	double m_zPos;
	double m_gas;

	void fillDataList();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedGcpPtBtn();
	afx_msg void OnBnClickedGenCadEntityBtn();
};
