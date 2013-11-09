#pragma once

#include "Resource.h"
#include "VNGParam.h"

// ͨ������ͼ���Ʋ�������
class GraphParamDlg : public CDialog
{
	DECLARE_DYNAMIC(GraphParamDlg)

public:
	GraphParamDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GraphParamDlg();

	enum { IDD = IDD_GRAPH_PARAM__DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	GraphParam gp;

private:
	double m_nodeSep;
	double m_rankSep;
	double m_graphRatio;
	double m_graphWidth;
	double m_graphHeight;
	BOOL m_useDefWH;

private:
	void togWHControls(BOOL flag);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedVngGenUseDefWh();
};
