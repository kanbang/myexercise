#pragma once

#include "resource.h"
// ChildDlg �Ի���

class ChildDlg : public CDialog
{
    DECLARE_DYNAMIC(ChildDlg)

public:
    ChildDlg(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~ChildDlg();

// �Ի�������
    enum { IDD = IDD_DIALOG1 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnCbnSelchangeCombo1();
};
