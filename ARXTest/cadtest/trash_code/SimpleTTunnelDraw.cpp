#include "StdAfx.h"
#include "SimpleTTunnelDraw.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleTTunnelDraw, MineGEDraw, 1 )

SimpleTTunnelDraw::SimpleTTunnelDraw( void ) : MineGEDraw()
{
}

SimpleTTunnelDraw::~SimpleTTunnelDraw( void )
{
}

void SimpleTTunnelDraw::setAllExtraParamsToDefault()
{
    m_gap = 30;
    m_width = 100;
    m_distance = 70;
}

void SimpleTTunnelDraw::configExtraParams()
{

}

void SimpleTTunnelDraw::updateExtraParams()
{

}

void SimpleTTunnelDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

void SimpleTTunnelDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void SimpleTTunnelDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_gap );
    reader.readDouble( m_width );
    reader.readDouble( m_distance );
}

void SimpleTTunnelDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_gap );
    writer.writeDouble( m_width );
    writer.writeDouble( m_distance );
}

void SimpleTTunnelDraw::drawLine( AcGiWorldDraw* mode, AcGePoint3d& startPt, AcGePoint3d& endPt )
{
    AcGePoint3dArray pts;
    pts.append( startPt );
    pts.append( endPt );

    mode->geometry().worldLine( pts.asArrayPtr() ); // 绘制直线
}

static void DrawMText( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, const CString& str, double height )
{
    AcDbMText mt;
    //AcDbObjectId style; // 文字样式
    //pMText->setTextStyle(style);
    mt.setContents( str );
    mt.setLocation( pt );
    mt.setTextHeight( height );
    mt.setAttachment( AcDbMText::kBottomCenter );
    mt.setRotation( angle );
    mt.worldDraw( mode );
}

void SimpleTTunnelDraw::drawText( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double angle, double height )
{
    DrawMText( mode, insertPt, angle, _T( "掘" ), height );
}

Adesk::Boolean SimpleTTunnelDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis ); // 得到原有的倾角向量

    AcGePoint3d basePt = m_insertPt + v * m_gap;
    AcGePoint3d pt3 = basePt + v * m_distance; // 远端点(三角形尖)

    // 绘制一个"掘"字
    double c = 0.618;
    double a = c * ( m_distance * m_width ) / ( m_distance + m_width );
    drawText( mode, basePt + v * a * 0.5, m_angle + 1.5 * PI, a );

    v.rotateBy( PI / 2, AcGeVector3d::kZAxis );
    AcGePoint3d pt1 = basePt + v * m_width / 2;

    v.rotateBy( PI, AcGeVector3d::kZAxis );
    AcGePoint3d pt2 = basePt + v * m_width / 2;

    drawLine( mode, pt1, pt2 );
    drawLine( mode, pt1, pt3 );
    drawLine( mode, pt2, pt3 );

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleTTunnelDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_insertPt.transformBy( xform );

    // 构造一个倾角向量
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis ); // 得到原有的倾角向量

    // 执行变换
    v.transformBy( xform );

    m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    return Acad::eOk;
}

void SimpleTTunnelDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{

}