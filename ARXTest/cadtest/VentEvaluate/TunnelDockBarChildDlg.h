#pragma once

#include "PropertyData_DockBarChildDlg.h"

class TunnelDockBarChildDlg : public PropertyData_DockBarChildDlg {
	DECLARE_DYNAMIC (TunnelDockBarChildDlg)

public:
	TunnelDockBarChildDlg (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;
	~TunnelDockBarChildDlg();

	enum { IDD = IDD_TUNNEL_PARAM_DLG};

public:
	virtual void DoDataExchange(CDataExchange *pDX) ;

protected:
	DECLARE_MESSAGE_MAP()

private:
	CString m_name;
	CString m_area;
	CString m_velocity;
	CString m_q;
	CString m_temp;
	CComboBox m_combo;
	CComboBox m_ttCombo;
	BOOL m_needAir;

protected:
	void readPropertyData();
	void writePropertyData();
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAfcBtn();
	afx_msg void OnBnClickedTunnelTypeNeedAir();
} ;
