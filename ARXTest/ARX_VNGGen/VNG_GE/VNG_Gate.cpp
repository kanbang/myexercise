#include "StdAfx.h"
#include "VNG_Gate.h"
#include "DrawTool.h"

Adesk::UInt32 VNG_Gate::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    VNG_Gate, VNG_GE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    构筑物, ARX_VNG_APP
)

VNG_Gate::VNG_Gate () : VNG_GE (), m_radius( 6 ), m_length( 8 )
{
}

VNG_Gate::~VNG_Gate ()
{
}

void VNG_Gate::setData( const AcGePoint3d& spt, const AcGePoint3d& pt, const AcGePoint3d& ept )
{
    assertWriteEnabled () ;

    m_insertPt = pt;
    m_angle = CacArcAngle( spt, ept, pt, m_radius );
}

Acad::ErrorStatus VNG_Gate::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = VNG_GE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( VNG_Gate::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writePoint3d( m_insertPt );
    pFiler->writeDouble( m_angle );
    pFiler->writeDouble( m_radius );
    pFiler->writeDouble( m_length );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus VNG_Gate::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = VNG_GE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > VNG_Gate::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    pFiler->readPoint3d( &m_insertPt );
    pFiler->readDouble( &m_angle );
    pFiler->readDouble( &m_radius );
    pFiler->readDouble( &m_length );

    return ( pFiler->filerStatus () ) ;
}

// 绘制一个半圆
static void DrawHalfCircle( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, double radius )
{
    AcGeVector3d sv( AcGeVector3d::kXAxis );
    sv.rotateBy( angle + PI / 2, AcGeVector3d::kZAxis );
    mode->geometry().circularArc( pt, radius, AcGeVector3d::kZAxis, sv, PI );
}

static void DrawTwoLine( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, double length )
{
    AcGeVector3d startVector( AcGeVector3d::kXAxis ), endVector( AcGeVector3d::kXAxis );

    // 注意：rotateBy函数同时会修改对应的this对象
    startVector.rotateBy( angle - PI / 2, AcGeVector3d::kZAxis );
    endVector.rotateBy( angle + PI / 2, AcGeVector3d::kZAxis );

    DrawLine( mode, pt, pt + startVector * length );
    DrawLine( mode, pt, pt + endVector * length );
}

Adesk::Boolean VNG_Gate::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    DrawHalfCircle( mode, m_insertPt, m_angle, m_radius ); // 绘制半圆
    DrawTwoLine( mode, m_insertPt, m_angle, m_length ); // 绘制两侧的直线

    return Adesk::kTrue;
}

Acad::ErrorStatus VNG_Gate::subTransformBy( const AcGeMatrix3d& xform )
{
    m_insertPt.transformBy( xform ); // 变化插入点

    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis );
    v.transformBy( xform );

    m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis ); // 变换后的旋转角度

    m_radius = v.length() * m_radius; // 变换后的半径
    m_length = v.length() * m_length; // 变换后的长度

    //AcGeVector3d v2(AcGeVector3d::kXAxis*m_length);
    //v2.rotateBy(PI/2+m_angle, AcGeVector3d::kZAxis);
    //v2.transformBy(xform);

    //m_length = v2.length(); // 变化后的长度

    return Acad::eOk;
}

//- Osnap points protocol
Acad::ErrorStatus VNG_Gate::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // 只捕捉1种类型的点：中点
    if( osnapMode != AcDb::kOsModeCen ) return Acad::eOk;

    if( osnapMode == AcDb::kOsModeCen )
    {
        snapPoints.append( m_insertPt );
    }

    return Acad::eOk;
}

//- Grip points protocol
Acad::ErrorStatus VNG_Gate::subGetGripPoints ( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // 插入点作为夹点
    gripPoints.append( m_insertPt );

    // 圆弧的的一个顶点
    AcGeVector3d v( AcGeVector3d::kXAxis * m_radius );
    v.rotateBy( m_angle + PI, AcGeVector3d::kZAxis );
    gripPoints.append( m_insertPt + v );

    // 直线的2个端点
    AcGeVector3d startVector( AcGeVector3d::kXAxis * m_length ),
                 endVector( AcGeVector3d::kXAxis * m_length );
    startVector.rotateBy( m_angle - PI / 2, AcGeVector3d::kZAxis );
    endVector.rotateBy( m_angle + PI / 2, AcGeVector3d::kZAxis );

    gripPoints.append( m_insertPt + startVector );
    gripPoints.append( m_insertPt + endVector );

    return Acad::eOk;
}

Acad::ErrorStatus VNG_Gate::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        // 插入点
        if ( idx == 0 ) m_insertPt += offset;

        // 圆弧顶点
        if ( idx == 1 )
        {
            // 计算圆弧半径
            AcGeVector3d v( AcGeVector3d::kXAxis * m_radius );
            v.rotateBy( m_angle + PI, AcGeVector3d::kZAxis );
            v += offset; // 半径向量进行偏移

            m_radius = v.length(); // 更新半径
        }

        if( idx == 2 )
        {
            AcGeVector3d v( AcGeVector3d::kXAxis );

            // 注意：rotateBy函数同时会修改对应的this对象
            v.rotateBy( m_angle - PI / 2, AcGeVector3d::kZAxis );
            v = v * m_length + v.dotProduct( offset ) * v; // 端点向量进行偏移

            m_length = v.length(); // 更新直线长度
        }

        if( idx == 3 )
        {
            AcGeVector3d v( AcGeVector3d::kXAxis );
            v.rotateBy( PI / 2 + m_angle, AcGeVector3d::kZAxis );
            v = v * m_length + v.dotProduct( offset ) * v; // 端点向量进行偏移

            m_length = v.length(); // 更新直线长度
        }
    }
    return Acad::eOk;
}

void VNG_Gate::setParam( double m_radius, double m_length )
{
    assertWriteEnabled () ;
    this->m_radius = m_radius;
    this->m_length = m_length;
}