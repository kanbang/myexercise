#pragma once

#include "resource.h"
#include "VNGParam.h"

class EdgeParamDlg : public CDialog
{
	DECLARE_DYNAMIC(EdgeParamDlg)

public:
	EdgeParamDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~EdgeParamDlg();

	enum { IDD = IDD_EDGE_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	EdgeParam ep;

private:
	double m_arrowWidth;
	double m_arrowLength;
	double m_edgeTextHeight;
	BOOL m_needEdge;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedEdgeDefBtn();
};
