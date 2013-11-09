#pragma once

#include "resource.h"
#include "afxcmn.h"

typedef struct
{
    LPTSTR pszLastName;
    LPTSTR pszFirstName;
    LPTSTR pszTerm;
} ITEMDATA, *PITEMDATA;

class TestDlg : public CDialog
{
    DECLARE_DYNAMIC(TestDlg)

public:
    TestDlg(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~TestDlg();

// 对话框数据
    enum { IDD = IDD_DIALOG2 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_list;
    ITEMDATA* m_pData[10];

    afx_msg void OnLvnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult);
    virtual BOOL OnInitDialog();
};
