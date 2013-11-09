#pragma once
#include "afxcmn.h"

#include <queue>

// ��־��Ϣ
struct LogMsg
{
    CString str;
    COLORREF color;
};
typedef std::queue<LogMsg> LogMsgQueue;

class CThreadTestDlg : public CDialog
{
// ����
public:
    CThreadTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
    enum { IDD = IDD_THREADTEST_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
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
    HANDLE m_hThread;  // ��¼��ǰ�߳̾��
    int m_step;        // ��ǰ�׶�
};
