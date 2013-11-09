#pragma once
#include "afxcmn.h"

#include <queue>

// 日志消息
struct LogMsg
{
    CString str;
    COLORREF color;
};
typedef std::queue<LogMsg> LogMsgQueue;

class CThreadTestDlg : public CDialog
{
// 构造
public:
    CThreadTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
    enum { IDD = IDD_THREADTEST_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();


    afx_msg LRESULT OnAppendLog(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnClearLog(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnCloseThread(WPARAM wParam,LPARAM lParam);

    DECLARE_MESSAGE_MAP()

private:
    int AppendToLog(CString str, COLORREF color);
    int AppendToLogAndScroll(CString str, COLORREF color);
    int GetNumVisibleLines(CRichEditCtrl* pCtrl);
    void test();
    //void sendLog();

public:
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
    CRichEditCtrl m_ctrlLog;
    HANDLE m_hThread;  // 记录当前线程句柄
    int m_step;        // 当前阶段
};
