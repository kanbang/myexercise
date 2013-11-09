#pragma once

#include "Resource.h"
#include "afxwin.h"
#include "ContourColorDlg.h"

class DrawContourDlg : public CDialog
{
	DECLARE_DYNAMIC(DrawContourDlg)

public:
	DrawContourDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DrawContourDlg();

// 对话框数据
	enum { IDD = IDD_DRAW_CONTOUR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_contourTypeCombo;
	int m_data_nCount;
	double m_data_maxZ;
	double m_data_minZ;

	CComboBox m_layerList;
	double m_maxZ;
	double m_minZ;
	double m_dz;

	void FillLayerList();
	void updateContourData(int gct);
	bool validateContourData();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeContourTypeCombo();
};
