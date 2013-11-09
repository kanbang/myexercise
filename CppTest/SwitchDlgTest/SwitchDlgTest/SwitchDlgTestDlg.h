
// SwitchDlgTestDlg.h : ͷ�ļ�
//

#pragma once

#include "Resource.h"
#include <vector>
typedef std::vector<CDialog*> DialogArray;

// CSwitchDlgTestDlg �Ի���
class CSwitchDlgTestDlg : public CDialog
{
// ����
public:
    CSwitchDlgTestDlg(CWnd* pParent = NULL);	// ��׼���캯��
    ~CSwitchDlgTestDlg();

// �Ի�������
    enum { IDD = IDD_SWITCHDLGTEST_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
    int mpos;        // ��ǰ��ʾ�ĶԻ�������
    DialogArray da;  // �Ի�������

public:
    void switchDlg(int pos);

// ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedPreBtn();
    afx_msg void OnBnClickedNextBtn();
};
