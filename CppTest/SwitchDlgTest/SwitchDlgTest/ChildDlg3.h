#pragma once


// ChildDlg3 �Ի���

class ChildDlg3 : public CDialog
{
    DECLARE_DYNAMIC(ChildDlg3)

public:
    ChildDlg3(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~ChildDlg3();

// �Ի�������
    enum { IDD = IDD_DIALOG3 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
};
