#pragma once


// CPrivate �Ի���
#include "Test_DialogDlg.h"

class CPrivate : public CDialog
{
    DECLARE_DYNAMIC(CPrivate)

public:
    CPrivate(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CPrivate();

// �Ի�������
    enum { IDD = IDD_DIALOG1 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    CString m_csabout;
protected:
    CTest_DialogDlg m_pCTestDlg;

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
public:
    CString m_teststr;
};
