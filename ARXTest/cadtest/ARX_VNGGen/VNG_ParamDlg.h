#pragma once

#include "Resource.h"

// ͨ������ͼ���Ʋ�������
class VNG_ParamDlg : public CDialog
{
	DECLARE_DYNAMIC(VNG_ParamDlg)

public:
	VNG_ParamDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~VNG_ParamDlg();

// �Ի�������
	enum { IDD = IDD_VNG_GEN_DLG };

public:
	double m_nodeSep;
	double m_rankSep;
	double m_graphRatio;
	double m_graphWidth;
	double m_graphHeight;
	BOOL m_useDefWH;

	double m_nodeWidth;
	double m_nodeHeight;
	double m_nodeTextHeight;

	double m_arrowWidth;
	double m_arrowLength;
	double m_edgeTextHeight;
	BOOL m_needEdge;

private:
	void togWHControls(BOOL flag);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedVngGenUseDefWh();
};
