// Test_DialogDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "ColorCombox.h"
#include "afxcmn.h"

// CTest_DialogDlg �Ի���
class CTest_DialogDlg : public CDialog
{
// ����
public:
    CTest_DialogDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
    enum { IDD = IDD_TEST_DIALOG_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
    void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);


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
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedTest();
    //afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
public:
    CColorCombox m_cmb_color;
public:
    afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
public:
    CSpinButtonCtrl m_spintest;
    CEdit m_edit;
public:
    int m_edit1;
    CString Get();
    CString m_cstest;
public:
    afx_msg void OnEnUpdateEdit();
public:
    afx_msg void OnBnClickedDialog();
};
