#pragma once


// ChildDlg1 对话框

class ChildDlg1 : public CDialog
{
    DECLARE_DYNAMIC(ChildDlg1)

public:
    ChildDlg1(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~ChildDlg1();

// 对话框数据
    enum { IDD = IDD_DIALOG1 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
};
