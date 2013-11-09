#pragma once
#include "afxwin.h"
#include "resource.h"

class ContourLayerDlg : public CDialog
{
	DECLARE_DYNAMIC(ContourLayerDlg)

public:
	ContourLayerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ContourLayerDlg();

// �Ի�������
	enum { IDD = IDD_DEL_CONTOUR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_layerList;
	void FillLayerList();

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
