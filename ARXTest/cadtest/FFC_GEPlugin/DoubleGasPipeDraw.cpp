#include "StdAfx.h"
#include "DoubleGasPipeDraw.h"

#include "DrawTool.h"
#include <cmath>

ACRX_CONS_DEFINE_MEMBERS ( DoubleGasPipeDraw, MineGEDraw, 1 )

DoubleGasPipeDraw::DoubleGasPipeDraw()
{
}

void DoubleGasPipeDraw::setAllExtraParamsToDefault()
{
    const double phi = 0.61803398827239717009900391523914;
    m_width = phi * 40;
    m_radius = 20 * phi;
}

void DoubleGasPipeDraw::configExtraParams()
{
    // 切换资源
    //CAcModuleResourceOverride myResources;
}

void DoubleGasPipeDraw::updateExtraParams()
{
    update();
}

void DoubleGasPipeDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_startPt );
    reader.readPoint( m_endPt );
}

void DoubleGasPipeDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_startPt );
    writer.writePoint( m_endPt );
}

void DoubleGasPipeDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_width );
    reader.readPoint( m_leftStartPt );
    reader.readPoint( m_leftEndPt );
    reader.readPoint( m_rightStartPt );
    reader.readPoint( m_rightEndPt );
    reader.readDouble( m_radius );
}

void DoubleGasPipeDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_width );
    writer.writePoint( m_leftStartPt );
    writer.writePoint( m_leftEndPt );
    writer.writePoint( m_rightStartPt );
    writer.writePoint( m_rightEndPt );
    writer.writeDouble( m_radius );
}

void DoubleGasPipeDraw::update()
{
    AcGeVector3d v = m_endPt - m_startPt;
    double angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
    caclLeftPoint( angle, m_leftStartPt, m_rightStartPt );
    caclRightPoint( angle, m_leftEndPt, m_rightEndPt );
}

void DoubleGasPipeDraw::caclLeftPoint( double angle, AcGePoint3d& startPt1, AcGePoint3d& startPt2 )
{
    startPt1.x = m_startPt.x - sin( angle ) * m_width / 2;
    startPt1.y = m_startPt.y + cos( angle ) * m_width / 2;

    startPt2.x = m_startPt.x + sin( angle ) * m_width / 2;
    startPt2.y = m_startPt.y - cos( angle ) * m_width / 2;
}

void DoubleGasPipeDraw::caclRightPoint( double angle, AcGePoint3d& endPt1, AcGePoint3d& endPt2 )
{
    endPt1.x = m_endPt.x - sin( angle ) * m_width / 2;
    endPt1.y = m_endPt.y + cos( angle ) * m_width / 2;

    endPt2.x = m_endPt.x + sin( angle ) * m_width / 2;
    endPt2.y = m_endPt.y - cos( angle ) * m_width / 2;
}

void DoubleGasPipeDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    pts.append( m_startPt );

    pts.append( m_leftStartPt );
    pts.append( m_leftEndPt );

    pts.append( m_endPt );

    pts.append( m_rightEndPt );
    pts.append( m_rightStartPt );
}

static void DrawSphereGasPipe( AcGiWorldDraw* mode, const AcGePoint3d& pt, double radius, const AcCmColor& c )
{
    AcCmColor bc;
    bc.setRGB( 0, 0, 0 ); // 使用黑色作为第2种颜色
    DrawCircleGradient( mode, pt, radius, ( "INVSPHERICAL" ), c, bc ); // // 反转球形
}

Adesk::Boolean DoubleGasPipeDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    DrawLine( mode, m_leftStartPt, m_leftEndPt );   // 绘制左线
    DrawLine( mode, m_rightStartPt, m_rightEndPt );	// 绘制右线

    // 绘制蓝色球形填充
    AcCmColor c;
    c.setRGB( 0, 0, 255 );
    DrawSphereGasPipe( mode, m_endPt, m_radius, c );

    // 绘制圆
    DrawCircle( mode, m_endPt, m_radius, false );

    return Adesk::kTrue;
}

Acad::ErrorStatus DoubleGasPipeDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_startPt.transformBy( xform );
    m_endPt.transformBy( xform );
    updateExtraParams();

    return Acad::eOk;
}

Acad::ErrorStatus DoubleGasPipeDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // 只捕捉1种类型的点：端点
    if( osnapMode != AcDb::kOsModeEnd )
        return Acad::eOk;

    Acad::ErrorStatus es = Acad::eOk;

    // 如果需要设定捕捉点位置，可能还需要增加一些捕捉点
    // 例如中点、2/3处点等等
    if ( osnapMode == AcDb::kOsModeEnd )
    {
        snapPoints.append( m_startPt );
        snapPoints.append( m_endPt );
    }

    return es;
}

//- Grip points protocol
Acad::ErrorStatus DoubleGasPipeDraw::subGetGripPoints ( AcGePoint3dArray& gripPoints,
        AcDbIntArray& osnapModes,
        AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_startPt );

    if( m_startPt == m_endPt )
    {
        AcGePoint3d pt( m_startPt );
        pt.x = pt.x + m_width * 0.3;
        gripPoints.append( pt );
    }
    else
    {
        gripPoints.append( m_endPt );
    }

    return Acad::eOk;
}

Acad::ErrorStatus DoubleGasPipeDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );

        if ( idx == 0 )
        {
            m_startPt += offset;
        }

        if ( idx == 1 )
        {
            m_endPt += offset;
        }
    }

    updateExtraParams();

    return Acad::eOk;
}