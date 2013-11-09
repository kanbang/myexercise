#include "stdafx.h"

static AcDbBlockTableRecord* openCurrentSpaceBlock( AcDb::OpenMode mode, AcDbDatabase* db )
{
    AcDbBlockTableRecord* blkRec;

    Acad::ErrorStatus es = acdbOpenObject( blkRec, db->currentSpaceId(), mode );
    if ( es != Acad::eOk )
        return NULL;
    else
        return blkRec;
}

bool PostGEToModelSpace( AcDbEntity* pEnt )
{
    AcDbBlockTableRecord* blkRec = openCurrentSpaceBlock(
                                       AcDb::kForWrite,
                                       acdbHostApplicationServices()->workingDatabase() );

    if ( blkRec == NULL ) return false;

    Acad::ErrorStatus es = blkRec->appendAcDbEntity( pEnt );
    if ( es != Acad::eOk ) return false;

    blkRec->close();

    return true;
}

void DrawEllipse( const AcGePoint3d& pt, double width, double height )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v *= width / 2;
    double ratio = height / width;
    AcDbEllipse* pEllipse = new AcDbEllipse( pt, AcGeVector3d::kZAxis, v, ratio );
    PostGEToModelSpace( pEllipse );
    pEllipse->close();
}

void DrawText( const AcGePoint3d& pt, const CString& text, double height, AcDb::TextHorzMode hm = AcDb::kTextLeft, AcDb::TextVertMode vm = AcDb::kTextBase )
{
    AcDbText* pText = new AcDbText();
    pText->setTextString( text );
    pText->setHeight( height );
    pText->setHorizontalMode( hm );
    pText->setVerticalMode( vm );
    /*
     * 根据AcDbText::setHorizontalMode()的说明
     * 如果设置为AcDb::kTextLeft，应使用setPosition()方法设置插入点
     * 如果设置为非AcDb::kTextLeft，应使用setAlignmentPoint()方法设置插入点
     * AcDbText::setVerticalMode()方法同上!!!
     */
    if( hm != AcDb::kTextLeft || vm == AcDb::kTextBase )
    {
        pText->setAlignmentPoint( pt );
    }
    else
    {
        pText->setPosition( pt );
    }
    // 废弃代码
    /*
    AcGeVector3d v1(AcGeVector3d::kXAxis), v2(AcGeVector3d::kXAxis);
    v1.rotateBy(PI, AcGeVector3d::kZAxis);
    v2.rotateBy(PI*1.5, AcGeVector3d::kZAxis);
    pText->setPosition(pt + (v1*width + v2*height)/2);

    pText->setAlignmentPoint(pt + (v2*height)/2);
    pText->setAlignmentPoint(pt);
    */

    PostGEToModelSpace( pText );
    pText->close();
}

void DrawMText( const AcGePoint3d& pt, double angle, const CString& text, double height )
{
    AcDbMText* pMText = new AcDbMText;
    //AcDbObjectId style; // 文字样式
    //pMText->setTextStyle(style);
    pMText->setLocation( pt );
    pMText->setRotation( angle );
    //pMText->setWidth(width); // 不设置宽度，自动调整
    pMText->setTextHeight( height );
    pMText->setAttachment( AcDbMText::kMiddleCenter ); // 默认居中
    pMText->setContents( text );

    PostGEToModelSpace( pMText );
    pMText->close();
}

void DrawLines( const AcGePoint3dArray& pts )
{
    // 绘制普通直线
    // 用多条直线连接成一条分支
    int len = pts.length();
    AcDbPolyline* pPline = new AcDbPolyline( len );
    for( int i = 0; i < len - 1; i++ )
    {
        AcDbLine* pLine = new AcDbLine( pts[i], pts[i + 1] );
        PostGEToModelSpace( pLine );
        pLine->close();
    }
}

void DrawPLines( const AcGePoint3dArray& pts, bool isClosed )
{
    // 绘制多段线
    int len = pts.length();
    if( len < 2 ) return;
    AcDbPolyline* pPline = new AcDbPolyline( len );
    for( int i = 0; i < len; i++ )
    {
        AcGePoint2d pt;
        pt.set( pts[i].x, pts[i].y );
        pPline->addVertexAt( i, pt );
    }
    pPline->setClosed( isClosed );
    PostGEToModelSpace( pPline );
    pPline->close();
}

void DrawSplines( const AcGePoint3dArray& pts )
{
    // 绘制样条曲线
    AcDbSpline* pSpline = new AcDbSpline( pts );
    PostGEToModelSpace( pSpline );
    pSpline->close();
}

static void Swap( double& a, double& b )
{
    double temp = a;
    a = b;
    b = temp;
}

void DrawArc( const AcGePoint3d& spt, const AcGePoint3d& pt, const AcGePoint3d& ept )
{
    AcGeCircArc3d arc( spt, pt, ept );
    AcGePoint3d cnt = arc.center();
    double radius = arc.radius();
    AcGeVector3d sv = spt - cnt;
    AcGeVector3d ev = ept - cnt;
    double sa = sv.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
    double ea = ev.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    if( arc.normal().z <= 0 )
    {
        Swap( sa, ea );
    }

    AcDbArc* pArc = new AcDbArc( cnt, radius, sa, ea );
    PostGEToModelSpace( pArc );
    pArc->close();
}

void DrawCircle( const AcGePoint3d& pt, double radius )
{
    AcDbCircle* pCircle = new AcDbCircle( pt, AcGeVector3d::kZAxis, radius );
    PostGEToModelSpace( pCircle );
    pCircle->close();
}