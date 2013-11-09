#pragma once

class ChildDlg2 : public CDialog
{
    DECLARE_DYNAMIC(ChildDlg2)

public:
    ChildDlg2(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~ChildDlg2();

// 对话框数据
    enum { IDD = IDD_DIALOG2 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
};
