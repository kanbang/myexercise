#pragma once

#include "resource.h"
#include "VNGParam.h"

class NodeParamDlg : public CDialog
{
	DECLARE_DYNAMIC(NodeParamDlg)

public:
	NodeParamDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~NodeParamDlg();

// �Ի�������
	enum { IDD = IDD_NODE_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	NodeParam np;

private:
	double m_nodeWidth;
	double m_nodeHeight;
	double m_nodeTextHeight;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedNodeDefBtn();
};
