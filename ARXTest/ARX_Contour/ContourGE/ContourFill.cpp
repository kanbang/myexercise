#include "StdAfx.h"
#include "ContourFill.h"

/* 全局函数(ARX_Contour.cpp) */
extern void ARX_SmoothPoints( const AcGePoint3dArray& inPts, AcGePoint3dArray& outPts );

Adesk::UInt32 ContourFill::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    ContourFill, ContourGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    等值线填充, CONTOURAPP )

ContourFill::ContourFill () : ContourGE (), m_bSmooth( true )
{
}

ContourFill::~ContourFill ()
{
}

Acad::ErrorStatus ContourFill::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = ContourGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( ContourFill::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeInt32( m_pts.length() );
    for( int i = 0; i < m_pts.length(); i++ )
    {
        pFiler->writePoint3d( m_pts[i] );
    }

    pFiler->writeInt32( m_rgb );
    pFiler->writeInt32( m_cnpts_num );
    pFiler->writeInt32( m_bpts_num );
    pFiler->writeBool( m_bSmooth );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus ContourFill::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = ContourGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > ContourFill::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    long n = 0;
    pFiler->readInt32( &n );
    m_pts.removeAll();
    for( int i = 0; i < ( int )n; i++ )
    {
        AcGePoint3d pt;
        pFiler->readPoint3d( &pt );
        m_pts.append( pt );
    }

    pFiler->readUInt32( &m_rgb );

    long i = 0;
    pFiler->readInt32( &i );
    m_cnpts_num = ( int )i;

    i = 0;
    pFiler->readInt32( &i );
    m_bpts_num = i;

    pFiler->readBool( &m_bSmooth );

    return ( pFiler->filerStatus() ) ;
}

static void CreateBoundaryLoop( const AcGePoint3dArray& pts, AcGeIntArray& edgeTypes, AcGeVoidPointerArray& edgePtrs )
{
    int n = pts.length();
    for( int i = 0; i < n - 1; i++ )
    {
        AcGePoint2d p1( pts[i].x, pts[i].y );
        AcGePoint2d p2( pts[i + 1].x, pts[i + 1].y );
        AcGeLineSeg2d* pLine = new AcGeLineSeg2d( p1, p2 );

        edgePtrs.append( pLine );
        edgeTypes.append( AcDbHatch::kLine );
    }
}

static void CreateSplineLoop( const AcGePoint3dArray& pts, AcGeIntArray& edgeTypes, AcGeVoidPointerArray& edgePtrs )
{
    // 转换成2d
    AcGePoint2dArray pts2;

    int n = pts.length();
    bool isClosed = false;

    if( pts.first() == pts.last() )
    {
        n--;
        isClosed = true;
    }

    for( int i = 0; i < n; i++ )
    {
        pts2.append( AcGePoint2d( pts[i].x, pts[i].y ) );
    }

    AcGeNurbCurve2d* pSpline = new AcGeNurbCurve2d( pts2 );
    if( isClosed )
    {
        pSpline->makeClosed();
    }

    edgePtrs.append( pSpline );
    edgeTypes.append( AcDbHatch::kSpline );
}

static void CreateContourLoop( const AcGePoint3dArray& pts, bool bSmooth,
                               AcGeIntArray& edgeTypes, AcGeVoidPointerArray& edgePtrs )
{
    if( bSmooth )
    {
        // 光滑
        AcGePoint3dArray spts;
        ARX_SmoothPoints( pts, spts );

        //CreateSplineLoop(spts, edgeTypes, edgePtrs);
        CreateBoundaryLoop( spts, edgeTypes, edgePtrs );
    }
    else
    {
        CreateBoundaryLoop( pts, edgeTypes, edgePtrs );
    }
}

static void CreateHatch( AcDbHatch* pHatch,
                         const AcGePoint3dArray& cnpts,
                         int cnpts_num, int bpts_num,
                         bool bSmooth )
{
    AcGeIntArray edgeTypes;
    AcGeVoidPointerArray edgePtrs;

    AcGePoint3dArray pts;

    // 最大外围边界
    if( cnpts_num == 0 && bpts_num > 0 )
    {
        for( int i = 0; i < bpts_num; i++ )
        {
            pts.append( cnpts[i] );
        }
        CreateBoundaryLoop( pts, edgeTypes, edgePtrs );
        pHatch->appendLoop( AcDbHatch::kDefault, edgePtrs, edgeTypes );
    }
    // 原始等值线闭合
    else if( cnpts[cnpts_num - 1] == cnpts.first() )
    {
        if( bpts_num > 0 )
        {
            // 包含外边界
            for( int i = cnpts_num; i < cnpts_num + bpts_num; i++ )
            {
                pts.append( cnpts[i] );
            }
            CreateBoundaryLoop( pts, edgeTypes, edgePtrs );
            pHatch->appendLoop( AcDbHatch::kDefault, edgePtrs, edgeTypes );
        }

        // 注意要清空edgeTypes和edgePtrs!!!
        pts.removeAll();
        edgeTypes.removeAll();
        edgePtrs.removeAll();
        for( int i = 0; i < cnpts_num; i++ )
        {
            pts.append( cnpts[i] );
        }
        CreateContourLoop( pts, bSmooth, edgeTypes, edgePtrs );
        pHatch->appendLoop( AcDbHatch::kDefault, edgePtrs, edgeTypes );
    }
    else
    {
        for( int i = 0; i < cnpts_num; i++ )
        {
            pts.append( cnpts[i] );
        }
        CreateContourLoop( pts, bSmooth, edgeTypes, edgePtrs );

        pts.removeAll();
        for( int i = cnpts_num - 1; i < cnpts_num + bpts_num; i++ )
        {
            pts.append( cnpts[i] );
        }
        CreateBoundaryLoop( pts, edgeTypes, edgePtrs );

        pHatch->appendLoop( AcDbHatch::kDefault, edgePtrs, edgeTypes );
    }
}

static void DrawContourFill( AcGiWorldDraw* mode,
                             const AcGePoint3dArray& cnpts,
                             int cnpts_num, int bpts_num,
                             int r, int g, int b,
                             bool bSmooth )
{
    if( cnpts.isEmpty() ) return;

    AcDbHatch hatch;

    // set color
    AcCmColor color;
    color.setRGB( r, g, b );
    hatch.setColor( color );

    // Set hatch plane
    hatch.setNormal( AcGeVector3d::kZAxis );
    hatch.setElevation( 0.0 );

    // Set non associative hatch
    hatch.setAssociative( Adesk::kFalse );

    // Set hatch pattern to SolidFill type
    hatch.setPattern( AcDbHatch::kPreDefined, _T( "SOLID" ) );

    // Set hatch style to kNormal
    hatch.setHatchStyle( AcDbHatch::kNormal );

    CreateHatch( &hatch, cnpts, cnpts_num, bpts_num, bSmooth );

    // Elaborate solid fill
    hatch.evaluateHatch();

    hatch.worldDraw( mode );
}

Adesk::Boolean ContourFill::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    DrawContourFill(
        mode,
        m_pts,
        m_cnpts_num, m_bpts_num,
        GetRValue( m_rgb ), GetGValue( m_rgb ), GetBValue( m_rgb ),
        m_bSmooth );

    return Adesk::kTrue;
}

void ContourFill::setPolygon( const AcGePoint3dArray& cnpts, int cnpts_num, int bpts_num, COLORREF rgb )
{
    assertWriteEnabled();

    m_pts.append( cnpts );
    m_cnpts_num = cnpts_num;
    m_bpts_num = bpts_num;
    m_rgb = rgb;
}

void ContourFill::smooth( bool bSmooth )
{
    assertWriteEnabled();

    this->m_bSmooth = bSmooth;
}
