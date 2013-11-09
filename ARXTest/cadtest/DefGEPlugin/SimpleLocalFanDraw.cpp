#include "StdAfx.h"
#include "SimpleLocalFanDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleLocalFanDraw, MineGEDraw, 1 )

SimpleLocalFanDraw::SimpleLocalFanDraw () : MineGEDraw ()
{
}

SimpleLocalFanDraw::~SimpleLocalFanDraw ()
{
}

void SimpleLocalFanDraw::setAllExtraParamsToDefault()
{
    m_radius = 40;
}

void SimpleLocalFanDraw::configExtraParams()
{

}

void SimpleLocalFanDraw::updateExtraParams()
{

}

void SimpleLocalFanDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

void SimpleLocalFanDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void SimpleLocalFanDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_radius );
}

void SimpleLocalFanDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_radius );
}

static AcGePoint3d CaclLeftBottomPt( const AcGePoint3d& pt, double angle, double width, double height )
{
    AcGeVector3d v( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );
    v2.rotateBy( angle + PI / 2, AcGeVector3d::kZAxis );
    if( v.x > 0 ) // 位于第1和4象限
    {
        v.rotateBy( PI, AcGeVector3d::kZAxis );
        v2.rotateBy( PI, AcGeVector3d::kZAxis );
    }
    return ( pt + v * width / 2 + v2 * height / 2 );
}

Adesk::Boolean SimpleLocalFanDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    // 绘制一个圆
    DrawCircle( mode, m_insertPt, m_radius, false );

    // 文字位于圆的内接正方形内
    double c = 0.70710678118654752440084436210485; // 根号2的倒数
    double height = 2 * m_radius * c;
    DrawMText( mode, m_insertPt, m_angle, _T( "F" ), height );

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleLocalFanDraw::subTransformBy( const AcGeMatrix3d& xform )
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

Acad::ErrorStatus SimpleLocalFanDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;
    // 只捕捉1种类型的点：插入点
    if( osnapMode != AcDb::kOsModeCen )
        return Acad::eOk;

    Acad::ErrorStatus es = Acad::eOk;

    if ( osnapMode == AcDb::kOsModeCen )
    {
        snapPoints.append( m_insertPt );
    }

    return es;
}

Acad::ErrorStatus SimpleLocalFanDraw::subGetGripPoints (
    AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds
) const
{
    assertReadEnabled () ;

    gripPoints.append( m_insertPt );

    return Acad::eOk;
}

Acad::ErrorStatus SimpleLocalFanDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        // 始节点
        if ( idx == 0 ) m_insertPt += offset;
    }
    return Acad::eOk;
}

static AcGePoint3d CaclPt( const AcGePoint3d& insertPt, const AcGeVector3d& v1, double width, const AcGeVector3d& v2, double height )
{
    return ( insertPt + v1 * width + v2 * height );
}

void SimpleLocalFanDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    /*AcGeVector3d v1(AcGeVector3d::kXAxis), v2(AcGeVector3d::kXAxis);
    v1.rotateBy(m_angle, AcGeVector3d::kZAxis);
    v2.rotateBy(m_angle+PI/2, AcGeVector3d::kZAxis);

    pts.append(CaclPt(m_insertPt, v1, m_radius, v2, m_radius));

    v1.rotateBy(PI, AcGeVector3d::kZAxis);
    pts.append(CaclPt(m_insertPt, v1, m_radius, v2, m_radius));

    v2.rotateBy(PI, AcGeVector3d::kZAxis);
    pts.append(CaclPt(m_insertPt, v1, m_radius, v2, m_radius));

    v1.rotateBy(PI, AcGeVector3d::kZAxis);
    pts.append(CaclPt(m_insertPt, v1, m_radius, v2, m_radius));*/
}