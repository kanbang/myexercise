
// ExpandDlgDlg.h : ͷ�ļ�
//

#pragma once

#include "ChildDlg.h"

// CExpandDlgDlg �Ի���
class CExpandDlgDlg : public CDialog
{
// ����
public:
    CExpandDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
    enum { IDD = IDD_EXPANDDLG_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

    ChildDlg m_childDlg;

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
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
};
