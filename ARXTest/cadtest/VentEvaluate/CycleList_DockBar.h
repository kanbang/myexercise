#pragma once

#include "acui.h"
#include "CycleListDlg.h"

class CycleList_DockBar : public CAcUiDockControlBar {
	DECLARE_DYNAMIC (CycleList_DockBar)

public:
	//----- Child dialog which will use the resource id supplied
	CycleListDlg mChildDlg ;

public:
	CycleList_DockBar ();
	virtual ~CycleList_DockBar ();

public:
	virtual BOOL Create (CWnd *pParent, LPCTSTR lpszTitle) ;
	virtual void SizeChanged (CRect *lpRect, BOOL bFloating, int flags) ;

	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct) ;
	afx_msg void OnSysCommand (UINT nID, LPARAM lParam) ;
	afx_msg void OnSize (UINT nType, int cx, int cy) ;

	// 处理dock bar关闭或隐藏时的情况
	virtual bool OnClosing ();

	DECLARE_MESSAGE_MAP()
} ;
