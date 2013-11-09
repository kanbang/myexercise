#pragma once


// ChildDlg3 对话框

class ChildDlg3 : public CDialog
{
    DECLARE_DYNAMIC(ChildDlg3)

public:
    ChildDlg3(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~ChildDlg3();

// 对话框数据
    enum { IDD = IDD_DIALOG3 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
};
