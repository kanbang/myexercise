// ContourView.cpp : implementation of the CContourView class
//

#include "stdafx.h"
#include "Contour.h"

#include "ContourDoc.h"
#include "ContourView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContourView

IMPLEMENT_DYNCREATE( CContourView, CLogScrollView )

BEGIN_MESSAGE_MAP( CContourView, CLogScrollView )
    //{{AFX_MSG_MAP(CContourView)
    ON_WM_MOUSEMOVE()
    ON_COMMAND( IDM_DRAWTRIANGLE, OnDrawtriangle )
    ON_COMMAND( IDM_SAVEMEMBER, OnSavemember )
    ON_WM_VSCROLL()
    ON_WM_HSCROLL()
    ON_WM_SIZE()
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
    // Standard printing commands
    ON_COMMAND( ID_FILE_PRINT, CView::OnFilePrint )
    ON_COMMAND( ID_FILE_PRINT_DIRECT, CView::OnFilePrint )
    ON_COMMAND( ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContourView construction/destruction

CContourView::CContourView()
{
    // TODO: add construction code here
    m_pTriangle = new CTriangle;
    tmpRatio = 50;  //比例因子，GetWindowsExt/GetViewPortExt的值, 相当与一个物理点代表多少个逻辑点
}

CContourView::~CContourView()
{
    if ( m_pOldBitmap != NULL )
    {
        m_memDC.SelectObject( m_pOldBitmap );
        m_memDC.DeleteDC();
    }

    delete m_pTriangle;
}


/////////////////////////////////////////////////////////////////////////////
// CContourView drawing

void CContourView::OnDraw( CDC* pDC )
{
    CContourDoc* pDoc = GetDocument();
    ASSERT_VALID( pDoc );

    // TODO: add draw code for native data here

    /////////////////////////////////////////////////////
    // 无效矩形区域不在内存平面上，修改内存片面的起始位置。
    // 实际修改内存SetWindowOrg()。
    // 在内存平面上，viewportorg永远为0，而windowsorg随时都会改变
    // 在窗口dc上，windowsorg永远为0, 而viewportorg随着滚动条改变
    // 在内存平面的右边或下边
    RECT rectClipBox;
    int theRet = pDC->GetClipBox( &rectClipBox );
    if ( theRet == ERROR || theRet == NULLREGION )
        return;
    CPoint ptWinOrg = m_memDC.GetWindowOrg();

    if( rectClipBox.left < ptWinOrg.x ||
            rectClipBox.right > ptWinOrg.x + m_cxBitmap * tmpRatio ||
            rectClipBox.top < ptWinOrg.y ||
            rectClipBox.bottom > ptWinOrg.y + m_cyBitmap * tmpRatio )
    {
        //设置内存平面新的窗口原点，使裁剪矩形的正中间位于内存位图正的中间
        m_memDC.SetWindowOrg( max( ( rectClipBox.left + rectClipBox.right ) / 2 - m_cxBitmap * tmpRatio / 2, 0 ),
                              max( ( rectClipBox.top + rectClipBox.bottom ) / 2 - m_cyBitmap * tmpRatio / 2, 0 ) );

        //***************************************
        //** Clear memDC's bitmap...
        CBrush backgroundBrush( ( COLORREF )::GetSysColor( COLOR_WINDOW ) );
        CBrush* pOldBrush = m_memDC.SelectObject( &backgroundBrush );
        ptWinOrg = m_memDC.GetWindowOrg(); //重新获得内存平面的新的窗口起始点
        m_memDC.PatBlt( ptWinOrg.x, ptWinOrg.y, m_cxBitmap * tmpRatio, m_cyBitmap * tmpRatio, PATCOPY );
        m_memDC.SelectObject( pOldBrush )->DeleteObject();
        //** end clear
        //***************************************

        // 重画内存位图
        m_pTriangle->DrawTriangle( &m_memDC );
        for ( int i = 1600; i < 2140; i += 45 )
            m_pTriangle->CreateContourEx( pDC, i );
    }
    // 内存平面的起始位置修改完毕！
    //////////////////////////////////////////////////////


    if( m_memDC.GetSafeHdc() != NULL && m_pTriangle->mydc != NULL )
    {
        CPoint pt3;
        pt3 = m_memDC.GetWindowOrg();
        pDC->BitBlt( pt3.x, pt3.y, m_cxBitmap * tmpRatio, m_cyBitmap * tmpRatio, &m_memDC, pt3.x, pt3.y, SRCCOPY );
    }

}


/////////////////////////////////////////////////////////////////////////////
// CContourView printing

BOOL CContourView::OnPreparePrinting( CPrintInfo* pInfo )
{
    // default preparation
    return DoPreparePrinting( pInfo );
}

void CContourView::OnBeginPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
{
    // TODO: add extra initialization before printing
}

void CContourView::OnEndPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
{
    // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CContourView diagnostics

#ifdef _DEBUG
void CContourView::AssertValid() const
{
    CView::AssertValid();
}

void CContourView::Dump( CDumpContext& dc ) const
{
    CLogScrollView::Dump( dc );
}

CContourDoc* CContourView::GetDocument() // non-debug version is inline
{
    ASSERT( m_pDocument->IsKindOf( RUNTIME_CLASS( CContourDoc ) ) );
    return ( CContourDoc* )m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CContourView message handlers

void CContourView::OnMouseMove( UINT nFlags, CPoint point )
{
    // TODO: Add your message handler code here and/or call default
    CDC*  dc;
    RECT therect;
    CString buffer;
    POINT pt( point );
    double zval;

    dc = this->GetDC();
    this->GetClientRect( &therect );
    dc->DPtoLP( &therect );

    dc->DPtoLP( &pt );
    buffer.Format( "                                                                                                                 " );
    TextOut( dc->m_hDC, therect.left, therect.top, buffer, buffer.GetLength() );
    if ( m_pTriangle->IsInControlDots( pt, zval ) == true )
        buffer.Format( "x=%7d  y=%7d  z=%7.2f", pt.x, pt.y, zval );
    else
        buffer.Format( "x=%7d  y=%7d", pt.x, pt.y );
    TextOut( dc->m_hDC, therect.left, therect.top, buffer, buffer.GetLength() );


    CLogScrollView::OnMouseMove( nFlags, point );
}

void CContourView::OnDrawtriangle()
{
    // TODO: Add your command handler code here
    CClientDC dc( this );
    CRect therect;
    this->GetClientRect( &therect );


    //***************************************
    //** Clear memDC's bitmap...
    CBrush backgroundBrush( ( COLORREF )::GetSysColor( COLOR_WINDOW ) );
    CBrush* pOldBrush = m_memDC.SelectObject( &backgroundBrush );
    m_memDC.PatBlt( 0, 0, m_cxBitmap, m_cyBitmap, PATCOPY );
    m_memDC.SelectObject( pOldBrush )->DeleteObject();
    //** end clear
    //***************************************

    m_pTriangle->InitialTriangle(); //Read source data...

    //设立坐标系
    m_memDC.SetMapMode( MM_ISOTROPIC );
    m_memDC.SetWindowExt( ( int )( m_pTriangle->xmax - m_pTriangle->xmin ), ( int )( m_pTriangle->ymax - m_pTriangle->ymin ) );
    m_memDC.SetViewportExt( ( int )( ( m_pTriangle->xmax - m_pTriangle->xmin ) / tmpRatio ), ( int )( ( m_pTriangle->ymax - m_pTriangle->ymin ) / tmpRatio ) );
    dc.SetMapMode( MM_ISOTROPIC );
    dc.SetWindowExt( ( int )( m_pTriangle->xmax - m_pTriangle->xmin ), ( int )( m_pTriangle->ymax - m_pTriangle->ymin ) );
    dc.SetViewportExt( ( int )( ( m_pTriangle->xmax - m_pTriangle->xmin ) / tmpRatio ), ( int )( ( m_pTriangle->ymax - m_pTriangle->ymin ) / tmpRatio ) );
    //两个坐标系统的重合点为原点为(0,0)

    // 写内存影像平面
    m_pTriangle->DrawTriangle( &m_memDC );
    //for (int i=1600; i<2140; i+=45)
    double zValues[4];
    zValues[0] = 1570;
    zValues[1] = 1590;
    zValues[2] = 1610;
    zValues[3] = 1630;
    for ( int i = 0; i < 4; i++ )
    {
        //m_pTriangle->CreateContourEx(&m_memDC, i);
        m_pTriangle->CreateContourEx( &m_memDC, zValues[i] );
    }


    SIZE sizeTotal;
    sizeTotal.cx = ( int )( m_pTriangle->xmax - m_pTriangle->xmin );
    sizeTotal.cy = ( int )( m_pTriangle->ymax - m_pTriangle->ymin );
    this->SetLogScrollSizes( sizeTotal, tmpRatio );
    ResizeParentToFit();


    //***************************************
    //**把影像从内存DC传递到设备DC
    dc.BitBlt( m_memDC.GetWindowOrg().x, m_memDC.GetWindowOrg().y, m_cxBitmap * tmpRatio, m_cyBitmap * tmpRatio, &m_memDC, m_memDC.GetWindowOrg().x, m_memDC.GetWindowOrg().y, SRCCOPY );
    //**
    //***************************************

    SCROLLINFO  scrollinfo;
    this->GetScrollInfo( SB_VERT, &scrollinfo );
}

void CContourView::OnSavemember()
{
    // TODO: Add your command handler code here
    CFile theFile;
    int theret = theFile.Open( "m_trisets.dat", CFile::modeCreate | CFile::modeWrite );
    CArchive ar( &theFile, CArchive::store );
    m_pTriangle->Serialize( ar );
    ar.Close();
    theFile.Close();
}



BOOL CContourView::PreCreateWindow( CREATESTRUCT& cs )
{
    // TODO: Add your specialized code here and/or call the base class

    // 使用一个持久的DC,即Private DC,  设置WNDCLS风格为CS_OWNDC
    cs.lpszClass = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC,
                                        ::LoadCursor( NULL, IDC_ARROW ), HBRUSH( COLOR_WINDOW + 1 ), NULL );

    return CLogScrollView::PreCreateWindow( cs );
}



void CContourView::OnInitialUpdate()
{
    CLogScrollView::OnInitialUpdate();

    // TODO: Add your specialized code here and/or call the base class
    CBitmap theBitmap;

    // Create the memory device context and the bitmap
    if( m_memDC.GetSafeHdc() == NULL )
    {
        CClientDC dc( this );
        m_memDC.CreateCompatibleDC( &dc );

        GetLargestDisplayMode( &m_cxBitmap, &m_cyBitmap );
        m_cxBitmap = min( m_cxBitmap * 2, 4096 );	//设置内存位图大小为显示设备最大分辨率的倍数；
        m_cyBitmap = min( m_cyBitmap * 2, 3072 );
        ASSERT( theBitmap.CreateCompatibleBitmap( &dc, m_cxBitmap, m_cyBitmap ) );

        m_pOldBitmap = m_memDC.SelectObject( &theBitmap );
    }

}


// 获得最大显示模式时屏幕的宽度和高度（以像素为单位）
void CContourView::GetLargestDisplayMode( int* pcxBitmap, int* pcyBitmap )
{
    DEVMODE devmode;
    int iModeNum = 0;
    ZeroMemory( &devmode, sizeof( DEVMODE ) );
    devmode.dmSize = sizeof( DEVMODE );

    while( EnumDisplaySettings( NULL, iModeNum++, &devmode ) )
    {
        *pcxBitmap = max( *pcxBitmap, ( int )devmode.dmPelsWidth );
        *pcyBitmap = max( *pcyBitmap, ( int )devmode.dmPelsHeight );
    }
}

void CContourView::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
    // TODO: Add your message handler code here and/or call default
    //ScrollToPosition();
    CLogScrollView::OnVScroll( nSBCode, nPos, pScrollBar );
}

void CContourView::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
    // TODO: Add your message handler code here and/or call default

    CLogScrollView::OnHScroll( nSBCode, nPos, pScrollBar );
}

void CContourView::OnSize( UINT nType, int cx, int cy )
{
    CLogScrollView::OnSize( nType, cx, cy );

    // TODO: Add your message handler code here

}


void CContourView::OnPaint()
{
    CPaintDC dc( this ); // device context for painting

    // TODO: Add your message handler code here
    OnPrepareDC( &dc );
    OnDraw( &dc );
    // Do not call CLogScrollView::OnPaint() for painting messages
}
