#include "StdAfx.h"
#include "SphereNitrogenPipeDraw.h"

#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( SphereNitrogenPipeDraw, MineGEDraw, 1 )

SphereNitrogenPipeDraw::SphereNitrogenPipeDraw()
{
}

SphereNitrogenPipeDraw::~SphereNitrogenPipeDraw( void )
{
}

void SphereNitrogenPipeDraw::setAllExtraParamsToDefault()
{
    const double phi = 0.61803399;
    m_radius = 20 * phi;
}

void SphereNitrogenPipeDraw::configExtraParams()
{

}

void SphereNitrogenPipeDraw::updateExtraParams()
{

}

void SphereNitrogenPipeDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_pt );
}

void SphereNitrogenPipeDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_pt );
}

void SphereNitrogenPipeDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_radius );
}

void SphereNitrogenPipeDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_radius );
}

static void DrawSphereNitrogenPipe( AcGiWorldDraw* mode, const AcGePoint3d& pt, double radius, const AcCmColor& c )
{
    AcCmColor bc;
    bc.setRGB( 0, 0, 0 ); // 使用黑色作为第2种颜色
    DrawCircleGradient( mode, pt, radius, ( "INVSPHERICAL" ), c, bc ); // // 反转球形
}

Adesk::Boolean SphereNitrogenPipeDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    // 绘制绿色球形填充
    AcCmColor c;
    c.setRGB( 0, 255, 0 );
    DrawSphereNitrogenPipe( mode, m_pt, m_radius, c );

    // 绘制圆
    DrawCircle( mode, m_pt, m_radius, false );

    return Adesk::kTrue;
}

Acad::ErrorStatus SphereNitrogenPipeDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    // 插入点变换
    m_pt.transformBy( xform );

    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.transformBy( xform );

//	m_radius *= v.length();

    return Acad::eOk;
}

Acad::ErrorStatus SphereNitrogenPipeDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;
    // 捕捉2种类型的点：端点和中心点
    if( osnapMode != AcDb::kOsModeCen )
        return Acad::eOk;

    Acad::ErrorStatus es = Acad::eOk;

    if( osnapMode == AcDb::kOsModeCen )
    {
        snapPoints.append( m_pt );
    }

    return es;
}

Acad::ErrorStatus SphereNitrogenPipeDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_pt );

    return Acad::eOk;
}

Acad::ErrorStatus SphereNitrogenPipeDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        if ( idx == 0 )
        {
            m_pt += offset; 			// 插入点偏移
        }
    }
    return Acad::eOk;
}
