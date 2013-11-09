#if !defined(AFX_COOLBAR_H__924D5C08_4172_4959_AE9F_9F4B0AFD3612__INCLUDED_)
#define AFX_COOLBAR_H__924D5C08_4172_4959_AE9F_9F4B0AFD3612__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoolBar dialog

class CCoolBar : public CDialog
{
// Construction
public:
    CCoolBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CCoolBar)
    enum { IDD = IDR_MAINFRAME };
    CString	m_staticxyz;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCoolBar)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CCoolBar)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLBAR_H__924D5C08_4172_4959_AE9F_9F4B0AFD3612__INCLUDED_)
