// Contour.h : main header file for the CONTOUR application
//

#if !defined(AFX_CONTOUR_H__CEE6B742_C32D_4479_9149_02306C23ACAA__INCLUDED_)
#define AFX_CONTOUR_H__CEE6B742_C32D_4479_9149_02306C23ACAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CContourApp:
// See Contour.cpp for the implementation of this class
//

class CContourApp : public CWinApp
{
public:
    CContourApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CContourApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation
    //{{AFX_MSG(CContourApp)
    afx_msg void OnAppAbout();
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTOUR_H__CEE6B742_C32D_4479_9149_02306C23ACAA__INCLUDED_)
