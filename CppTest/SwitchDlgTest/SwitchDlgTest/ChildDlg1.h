#pragma once


// ChildDlg1 �Ի���

class ChildDlg1 : public CDialog
{
    DECLARE_DYNAMIC(ChildDlg1)

public:
    ChildDlg1(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~ChildDlg1();

// �Ի�������
    enum { IDD = IDD_DIALOG1 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
};
