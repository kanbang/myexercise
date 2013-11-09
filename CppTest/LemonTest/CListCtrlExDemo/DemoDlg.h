// DemoDlg.h : header file
//

#pragma once
#include "listctrlex.h"
#include "afxdtctl.h"
#include "afxwin.h"
#include "dlgcolor.h"


// CDemoDlg dialog
class CDemoDlg : public CDialog
{
// Construction
public:
    CDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
    enum { IDD = IDD_CLISTCTRLEXDEMO_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
    HICON m_hIcon;
    static BOOL InitEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);
    static BOOL EndEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);
    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
    CImageList m_imgList;
public:
    CListCtrlEx m_lstDemo;
    void AddColumns(void);
    void FillListBox(void);
    CDateTimeCtrl m_wndDT;
    CComboBox m_wndCB;
    CEdit m_wndEdit;
    //CDlgColor m_dlgColor;
    CColorDialog m_dlgColor;
};
