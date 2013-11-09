#pragma once

#include "resource.h"

class RegionForecastDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(RegionForecastDlg)

public:
	RegionForecastDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~RegionForecastDlg();

// �Ի�������
	enum { IDD = IDD_REGION_FORECAST_DLG };

private:
	double m_thickness;
	double m_weight;
	double m_area;
	double m_gas;
	int m_count;

	AcGePoint3dArray m_polygon;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRegionCadEntity();
	afx_msg void OnBnClickedRegionSelectBtn();
};
