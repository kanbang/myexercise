
// SwitchDlgTestDlg.h : 头文件
//

#pragma once

#include "Resource.h"
#include <vector>
typedef std::vector<CDialog*> DialogArray;

// CSwitchDlgTestDlg 对话框
class CSwitchDlgTestDlg : public CDialog
{
// 构造
public:
    CSwitchDlgTestDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CSwitchDlgTestDlg();

// 对话框数据
    enum { IDD = IDD_SWITCHDLGTEST_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
    int mpos;        // 当前显示的对话框索引
    DialogArray da;  // 对话框数组

public:
    void switchDlg(int pos);

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
    afx_msg void OnBnClickedPreBtn();
    afx_msg void OnBnClickedNextBtn();
};
