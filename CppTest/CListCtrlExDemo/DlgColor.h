#pragma once


// CDlgColor dialog

class CDlgColor : public CDialog
{
    DECLARE_DYNAMIC(CDlgColor)

public:
    CDlgColor(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgColor();

// Dialog Data
    enum { IDD = IDD_DIALOGBAR };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    int m_nColor;
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
