// ContourView.h : interface of the CContourView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTOURVIEW_H__BB0849AD_2202_4F37_94D3_89F77F98BD0E__INCLUDED_)
#define AFX_CONTOURVIEW_H__BB0849AD_2202_4F37_94D3_89F77F98BD0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "logscrollview.h"

class CContourView : public CLogScrollView
{
protected: // create from serialization only
    CContourView();
    DECLARE_DYNCREATE(CContourView)

// Attributes
public:
    CContourDoc* GetDocument();

// Add by Robter
private:
    CDC m_memDC;
    CBitmap* m_pOldBitmap;
    int m_cxBitmap, m_cyBitmap;
private:
    CTriangle* m_pTriangle;

private:
    //�ڸ���ͬ��(MM_ISOMETRIC)��ӳ��ģʽ�У�ϵͳ��ȡ��x��y�����б�����������ֵ��Ϊ��׼
    int tmpRatio; //�������ӣ�GetWindowsExt/GetViewPortExt��ֵ

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CContourView)
public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual void OnInitialUpdate();
protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

// Implementation
public:
    void GetLargestDisplayMode(int* pcxBitmap, int* pcyBitmap);// ��������ʾģʽʱ��Ļ�Ŀ�Ⱥ͸߶ȣ�������Ϊ��λ��
    virtual ~CContourView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CContourView)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnDrawtriangle();
    afx_msg void OnSavemember();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ContourView.cpp
inline CContourDoc* CContourView::GetDocument()
{
    return (CContourDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTOURVIEW_H__BB0849AD_2202_4F37_94D3_89F77F98BD0E__INCLUDED_)
