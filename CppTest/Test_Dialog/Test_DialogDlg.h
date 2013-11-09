// Test_DialogDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "ColorCombox.h"
#include "afxcmn.h"

// CTest_DialogDlg 对话框
class CTest_DialogDlg : public CDialog
{
// 构造
public:
    CTest_DialogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
    enum { IDD = IDD_TEST_DIALOG_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
    void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);


// 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedTest();
    //afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
public:
    CColorCombox m_cmb_color;
public:
    afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
public:
    CSpinButtonCtrl m_spintest;
    CEdit m_edit;
public:
    int m_edit1;
    CString Get();
    CString m_cstest;
public:
    afx_msg void OnEnUpdateEdit();
public:
    afx_msg void OnBnClickedDialog();
};
