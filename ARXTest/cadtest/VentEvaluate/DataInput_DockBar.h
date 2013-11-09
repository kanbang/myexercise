#pragma once

#include "acui.h"
#include "TunnelDockBarChildDlg.h"
#include "WSDockBarChildDlg.h"
#include "TWSDockBarChildDlg.h"
#include "RoomDockBarChildDlg.h"

#include "HelpDlg.h"

enum DI_TYPE
{
	DI_TUNNEL = 0,
	DI_WS     = 1,
	DI_TWS    = 2,
	DI_ROOM   = 3,
	DI_HELP   = 4
};

class DataInput_DockBar : public CAcUiDockControlBar 
{
	DECLARE_DYNAMIC (DataInput_DockBar)

public:
	DataInput_DockBar ();
	virtual ~DataInput_DockBar ();

	void update(DI_TYPE di, const AcDbObjectId& objId);

private:
	TunnelDockBarChildDlg m_ttChildDlg;
	WSDockBarChildDlg m_wsChildDlg;
	TWSDockBarChildDlg m_twsChildDlg;
	RoomDockBarChildDlg m_roomChildDlg;
	HelpDlg m_helpDlg;

	PropertyData_DockBarChildDlg* dlgArray[5]; // 辅助用
	void showDlg(DI_TYPE di);

	AcDbObjectId m_objId;
	DI_TYPE m_di;  // 默认DI_HELP

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
