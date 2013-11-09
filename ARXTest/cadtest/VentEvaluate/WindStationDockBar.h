#pragma once

#include "acui.h"
#include "WindStationDockBarChildDlg.h"

class WindStationDockBar : public CAcUiDockControlBar {
	DECLARE_DYNAMIC (WindStationDockBar)

public:
	//----- Child dialog which will use the resource id supplied
	WindStationDockBarChildDlg mChildDlg ;

public:
	WindStationDockBar ();
	virtual ~WindStationDockBar ();

public:
	virtual BOOL Create (CWnd *pParent, LPCTSTR lpszTitle) ;
	virtual void SizeChanged (CRect *lpRect, BOOL bFloating, int flags) ;

	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct) ;
	afx_msg void OnSysCommand (UINT nID, LPARAM lParam) ;
	afx_msg void OnSize (UINT nType, int cx, int cy) ;

	// ����dock bar�رջ�����ʱ�����
	virtual bool OnClosing ();

	DECLARE_MESSAGE_MAP()
} ;
