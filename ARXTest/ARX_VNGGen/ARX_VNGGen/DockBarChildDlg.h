#pragma once

#include "acui.h"
#include "Resource.h"

// ����ģ̬�Ի����3����Ϣ�����˻���DockBarChildDlg
// ����������Ϳ���ֻרע�Ի��������Ϣ����
// ͬʱ�����ظ�����
class DockBarChildDlg : public CAcUiDialog 
{
	DECLARE_DYNAMIC (DockBarChildDlg)

protected:
	DockBarChildDlg(UINT idd, CWnd *pParent =NULL, HINSTANCE hInstance = NULL) ;
	~DockBarChildDlg();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnClosing(); // Ĭ��ʵ��Ϊ��(�����Ҫ����رգ����ظú���)

	virtual BOOL OnCommand (WPARAM wParam, LPARAM lParam) ;
	afx_msg void OnSize (UINT nType, int cx, int cy) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM wParam, LPARAM lParam) ;
} ;
