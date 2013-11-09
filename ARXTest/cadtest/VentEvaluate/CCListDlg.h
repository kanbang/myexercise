#pragma once
#include "afxcmn.h"

#include "Resource.h"

class CCListDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CCListDlg)

public:
	CCListDlg(CWnd *pParent =NULL, HINSTANCE hInstance =NULL);   // 标准构造函数
	virtual ~CCListDlg();

// 对话框数据
	enum { IDD = IDD_CC_LIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	bool m_listFinishInitial; // 表示listctrl是否初始化完毕???
	AcArray<Adesk::UInt16> m_colors;
	AcDbObjectIdArray m_objIds;
	AcDbIntArray m_ccIds;
	int m_lastPos;
	CListCtrl m_list;

private:
	void initTunnelList();
	void clearAll();
	void findEdgeByCC(int cc, int& startPos, int& endPos);

public:
	// 处理dock bar关闭或隐藏时的情况
	/* 手动添加，不是从基类派生的 */
	void OnClosing ();

	virtual BOOL OnCommand (WPARAM wParam, LPARAM lParam) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM wParam, LPARAM lParam) ;
	afx_msg void OnSize (UINT nType, int cx, int cy) ;

	afx_msg void OnBnClickedCCListUpdateBtn();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};
