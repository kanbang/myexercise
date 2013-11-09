#include "StdAfx.h"
#include "SimpleSandWallDraw.h"
#include "DrawTool.h"

/* 全局函数(HatchRatio, 计算填充比例) */
extern double CaclHatchRatio( const AcGePoint3dArray& polygon );

ACRX_CONS_DEFINE_MEMBERS ( SimpleSandWallDraw, MineGEDraw, 1 )

SimpleSandWallDraw::SimpleSandWallDraw () : MineGEDraw ()
{
}

SimpleSandWallDraw::~SimpleSandWallDraw ()
{
}

void SimpleSandWallDraw::setAllExtraParamsToDefault()
{
    const double phi = 1.61803398827239717009900391523914;
    m_thick = 40 * phi;
    needFill = false;
    m_scale = 1.0;
}

void SimpleSandWallDraw::configExtraParams()
{

}

void SimpleSandWallDraw::updateExtraParams()
{
    AcGePoint3dArray pts;
    BuildRect( m_pt, m_angle, m_thick, m_length, pts );

    m_scale = CaclHatchRatio( pts );
}

void SimpleSandWallDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_pt );
    writer.writeDouble( m_angle );
    writer.writeDouble( m_length );
}

void SimpleSandWallDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_pt );
    reader.readDouble( m_angle );
    reader.readDouble( m_length );
}

void SimpleSandWallDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_thick );
    reader.readBoolean( needFill );
    reader.readDouble( m_scale );
}

void SimpleSandWallDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_thick );
    writer.writeBoolean( needFill );
    writer.writeDouble( m_scale );
}

static void DrawWall( AcGiWorldDraw* mode,
                      const AcGePoint3d& pt, double angle,
                      double thick, double length, double scale )
{
    AcGePoint3dArray pts;
    BuildRect( pt, angle, thick, length, pts );
    DrawPolygonHatch( mode, pts, _T( "AR-SAND" ), scale ); // 沙墙
}

Adesk::Boolean SimpleSandWallDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    // 绘制矩形块，并填充
    DrawRect( mode, m_pt, m_angle, m_thick, m_length, needFill );

    //acutPrintf(_T("\nscale:%.5f"), m_scale);

    // 适当缩小scale
    //const double phi = 0.61803398827239717009900391523914;
    const double c = 0.35;

    // 绘制墙
    DrawWall( mode, m_pt, m_angle, m_thick, m_length, c * m_scale );

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleSandWallDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_pt.transformBy( xform ); // 变化插入点

    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis );
    v.transformBy( xform );
    m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis ); // 变换后的旋转角度

    m_length *= v.length();
    //m_thick *= v.length();  // 墙宽度变换，如果不需要可以注释掉

    return Acad::eOk;
}

Acad::ErrorStatus SimpleSandWallDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    if( osnapMode == AcDb::kOsModeCen )
    {
        snapPoints.append( m_pt );
    }
    else
    {
        AcGeVector3d v( AcGeVector3d::kXAxis );
        v.rotateBy( m_angle + PI * 0.5, AcGeVector3d::kZAxis );
        snapPoints.append( m_pt + v * m_length * 0.5 );

        v.rotateBy( PI, AcGeVector3d::kZAxis );
        snapPoints.append( m_pt + v * m_length * 0.5 );
    }

    return Acad::eOk;
}

//- Grip points protocol
Acad::ErrorStatus SimpleSandWallDraw::subGetGripPoints ( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_pt );

    AcGeVector3d v1( AcGeVector3d::kXAxis );

    v1.rotateBy( m_angle + PI * 0.5, AcGeVector3d::kZAxis );
    gripPoints.append( m_pt + v1 * m_length * 0.5 );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    gripPoints.append( m_pt + v1 * m_length * 0.5 );

    // 墙宽度变化夹点
    // 如果不需要可以注释掉
    //AcGeVector3d v2(AcGeVector3d::kXAxis);
    //v2.rotateBy(m_angle, AcGeVector3d::kZAxis);
    //gripPoints.append(m_pt+v2*m_thick*0.5);

    //v2.rotateBy(PI, AcGeVector3d::kZAxis);
    //gripPoints.append(m_pt+v2*m_thick*0.5);

    return Acad::eOk;
}

Acad::ErrorStatus SimpleSandWallDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled() ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        if ( idx == 0 )
        {
            m_pt += offset;
        }
        else if( idx == 1 )
        {
            AcGeVector3d v( AcGeVector3d::kXAxis );
            v.rotateBy( m_angle + PI * 0.5, AcGeVector3d::kZAxis );
            v = v * m_length * 0.5 + v.dotProduct( offset ) * v;
            if( v != AcGeVector3d::kIdentity )
            {
                m_length = v.length() * 2;
            }
        }
        else if( idx == 2 )
        {
            AcGeVector3d v( AcGeVector3d::kXAxis );
            v.rotateBy( m_angle + PI * 1.5, AcGeVector3d::kZAxis );
            v = v * m_length * 0.5 + v.dotProduct( offset ) * v;

            if( v != AcGeVector3d::kIdentity )
            {
                m_length = v.length() * 2;
            }
        }
        // 墙宽度变化夹点
        // 如果不需要可以注释掉
        //else if(idx == 3)
        //{
        //	AcGeVector3d v(AcGeVector3d::kXAxis);
        //	v.rotateBy(m_angle, AcGeVector3d::kZAxis);
        //	v = v*m_thick*0.5 + v.dotProduct(offset)*v;
        //	if(v != AcGeVector3d::kIdentity)
        //	{
        //		m_thick = v.length()*2;
        //	}
        //}
        //else if(idx == 4)
        //{
        //	AcGeVector3d v(AcGeVector3d::kXAxis);
        //	v.rotateBy(m_angle+PI, AcGeVector3d::kZAxis);
        //	v = v*m_thick*0.5 + v.dotProduct(offset)*v;

        //	if(v != AcGeVector3d::kIdentity)
        //	{
        //		m_thick = v.length()*2;
        //	}
        //}
    }

    updateExtraParams();

    return Acad::eOk;
}

void SimpleSandWallDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    BuildRect( m_pt, m_angle, m_thick, m_length, pts );
}
