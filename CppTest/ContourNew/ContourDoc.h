// ContourDoc.h : interface of the CContourDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTOURDOC_H__F6941720_709C_4F45_945B_663E7DE4FDB2__INCLUDED_)
#define AFX_CONTOURDOC_H__F6941720_709C_4F45_945B_663E7DE4FDB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "triangle.h"
#include "boundary.h"


class CContourDoc : public CDocument
{
protected: // create from serialization only
    CContourDoc();
    DECLARE_DYNCREATE(CContourDoc)

// Attributes
public:
//	CTriangle m_Triangle;


// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CContourDoc)
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CContourDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CContourDoc)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTOURDOC_H__F6941720_709C_4F45_945B_663E7DE4FDB2__INCLUDED_)
