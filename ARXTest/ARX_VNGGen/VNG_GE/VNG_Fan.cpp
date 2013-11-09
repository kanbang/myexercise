#include "StdAfx.h"
#include "VNG_Fan.h"
#include "DrawTool.h"

Adesk::UInt32 VNG_Fan::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    VNG_Fan, VNG_GE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    通风动力, ARX_VNG_APP
)

VNG_Fan::VNG_Fan () : VNG_GE (), m_radius( 6 ), m_distance( 22 )
{
}

VNG_Fan::~VNG_Fan ()
{
}

void VNG_Fan::setData( const AcGePoint3d& spt, const AcGePoint3d& pt, const AcGePoint3d& ept )
{
    assertWriteEnabled () ;

    m_insertPt = pt;
    m_angle = CacArcAngle( spt, ept, pt, m_radius );
}

Acad::ErrorStatus VNG_Fan::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = VNG_GE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( VNG_Fan::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writePoint3d( m_insertPt );
    pFiler->writeDouble( m_angle );
    pFiler->writeDouble( m_radius );
    pFiler->writeDouble( m_distance );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus VNG_Fan::dwgInFields ( AcDbDwgFiler* pFiler )
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
    if ( version > VNG_Fan::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    pFiler->readPoint3d( &m_insertPt );
    pFiler->readDouble( &m_angle );
    pFiler->readDouble( &m_radius );
    pFiler->readDouble( &m_distance );

    return ( pFiler->filerStatus () ) ;
}

Adesk::Boolean VNG_Fan::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    DrawCircle( mode, m_insertPt, m_radius, false );

    // 计算点坐标
    AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    v1.rotateBy( m_angle + PI / 6, AcGeVector3d::kZAxis ); // 倾角：30度
    v2.rotateBy( m_angle - PI / 6, AcGeVector3d::kZAxis );

    double c = 1.1547005383792515290182975610039; // 2除以根号3(等价于sin(60)的倒数)
    double L = c * m_distance;
    AcGePoint3d firstPt = m_insertPt + v1 * L;
    AcGePoint3d secondPt = m_insertPt + v2 * L;

    // 绘制3条直线
    DrawLine( mode, m_insertPt, firstPt );
    DrawLine( mode, m_insertPt, secondPt );
    DrawLine( mode, firstPt, secondPt );

    return Adesk::kTrue;
}

Acad::ErrorStatus VNG_Fan::subTransformBy( const AcGeMatrix3d& xform )
{
    m_insertPt.transformBy( xform );

    // 构造一个倾角向量
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis ); // 得到原有的倾角向量

    // 执行变换
    v.transformBy( xform );

    m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    m_radius = v.length() * m_radius;
    m_distance = v.length() * m_distance;

    return Acad::eOk;
}

Acad::ErrorStatus VNG_Fan::subGetOsnapPoints (
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

        //AcGeVector3d v(AcGeVector3d::kXAxis);
        //v.rotateBy(m_angle, AcGeVector3d::kZAxis);

        //snapPoints.append(m_insertPt+v*m_distance);
    }

    return es;
}

Acad::ErrorStatus VNG_Fan::subGetGripPoints ( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_insertPt );

    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis );

    gripPoints.append( m_insertPt + v * m_radius );
    gripPoints.append( m_insertPt + v * m_distance );

    return Acad::eOk;
}

Acad::ErrorStatus VNG_Fan::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        // 始节点
        if ( idx == 0 ) m_insertPt += offset;

        if ( idx == 1 )
        {
            AcGeVector3d v( AcGeVector3d::kXAxis * m_radius );
            v.rotateBy( m_angle, AcGeVector3d::kZAxis );

            v += offset;
            m_radius = v.length();
        }
        if( idx == 2 )
        {
            AcGeVector3d v( AcGeVector3d::kXAxis * m_distance );
            v.rotateBy( m_angle, AcGeVector3d::kZAxis );

            v += offset;

            m_distance = v.length();
            m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
        }
    }
    return Acad::eOk;
}

void VNG_Fan::setParam( double m_radius, double m_distance )
{
    assertWriteEnabled () ;
    this->m_radius = m_radius;
    this->m_distance = m_distance;
}