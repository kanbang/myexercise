#pragma once

#include "resource.h"
// ChildDlg 对话框

class ChildDlg : public CDialog
{
    DECLARE_DYNAMIC(ChildDlg)

public:
    ChildDlg(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~ChildDlg();

// 对话框数据
    enum { IDD = IDD_DIALOG1 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnCbnSelchangeCombo1();
};
