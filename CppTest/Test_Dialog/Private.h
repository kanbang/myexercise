#pragma once


// CPrivate 对话框
#include "Test_DialogDlg.h"

class CPrivate : public CDialog
{
    DECLARE_DYNAMIC(CPrivate)

public:
    CPrivate(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CPrivate();

// 对话框数据
    enum { IDD = IDD_DIALOG1 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    CString m_csabout;
protected:
    CTest_DialogDlg m_pCTestDlg;

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
public:
    CString m_teststr;
};
