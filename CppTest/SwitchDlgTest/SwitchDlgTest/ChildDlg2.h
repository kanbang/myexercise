#pragma once

class ChildDlg2 : public CDialog
{
    DECLARE_DYNAMIC(ChildDlg2)

public:
    ChildDlg2(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~ChildDlg2();

// �Ի�������
    enum { IDD = IDD_DIALOG2 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
};
