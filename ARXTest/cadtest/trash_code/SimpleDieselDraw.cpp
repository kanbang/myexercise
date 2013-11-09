#include "StdAfx.h"
#include "SimpleDieselDraw.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleDieselDraw, MineGEDraw, 1 )

//-----------------------------------------------------------------------------
SimpleDieselDraw::SimpleDieselDraw () : MineGEDraw ()
{
}

SimpleDieselDraw::~SimpleDieselDraw ()
{
}

void SimpleDieselDraw::setAllExtraParamsToDefault()
{
    m_radius = 30;
    m_distance = 120;
}

void SimpleDieselDraw::configExtraParams()
{

}

void SimpleDieselDraw::updateExtraParams()
{

}

void SimpleDieselDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

void SimpleDieselDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void SimpleDieselDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_radius );
    reader.readDouble( m_distance );
}

void SimpleDieselDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_radius );
    writer.writeDouble( m_distance );
}

Adesk::Boolean SimpleDieselDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    // ���������
    AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    v1.rotateBy( m_angle, AcGeVector3d::kZAxis );
    v2.rotateBy( m_angle + PI, AcGeVector3d::kZAxis );

    AcGePoint3d firstPt = m_insertPt + v1 * m_distance / 2;
    AcGePoint3d secondPt = m_insertPt + v2 * m_distance / 2;

    AcGePoint3dArray pts;
    pts.append( firstPt );
    pts.append( secondPt );
    mode->geometry().worldLine( pts.asArrayPtr() );	                 // ����ֱ��

    AcGiSubEntityTraits& traits = mode->subEntityTraits();
    Adesk::UInt16 cl = traits.color();;
    AcGiFillType ft = traits.fillType();

    AcCmEntityColor bgColor;
    bgColor.setRGB( 255, 255, 0 ); // ��ɫ(yellow)
    traits.setTrueColor( bgColor );
    traits.setFillType( kAcGiFillAlways ); // ���

    mode->geometry().circle( firstPt, m_radius, AcGeVector3d::kZAxis ); // ����һ��Բ
    mode->geometry().circle( secondPt, m_radius, AcGeVector3d::kZAxis ); // ����һ��Բ

    // �ָ�����
    traits.setFillType( ft );
    traits.setColor( cl );

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleDieselDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_insertPt.transformBy( xform );

    // ����һ���������
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis ); // �õ�ԭ�е��������

    // ִ�б任
    v.transformBy( xform );

    m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    return Acad::eOk;
}

Acad::ErrorStatus SimpleDieselDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;
    // ֻ��׽1�����͵ĵ㣺�����
    if( osnapMode != AcDb::kOsModeCen )
        return Acad::eOk;

    Acad::ErrorStatus es = Acad::eOk;

    if ( osnapMode == AcDb::kOsModeCen )
    {
        snapPoints.append( m_insertPt );
    }

    return es;
}

Acad::ErrorStatus SimpleDieselDraw::subGetGripPoints (
    AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds
) const
{
    assertReadEnabled () ;

    gripPoints.append( m_insertPt );

    return Acad::eOk;
}

Acad::ErrorStatus SimpleDieselDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        // ʼ�ڵ�
        if ( idx == 0 ) m_insertPt += offset;
    }
    return Acad::eOk;
}