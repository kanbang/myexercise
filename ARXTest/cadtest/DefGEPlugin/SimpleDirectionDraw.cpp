#include "StdAfx.h"
#include "SimpleDirectionDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleDirectionDraw, MineGEDraw, 1 )

SimpleDirectionDraw::SimpleDirectionDraw()
{
}

SimpleDirectionDraw::~SimpleDirectionDraw( void )
{
}

void SimpleDirectionDraw::setAllExtraParamsToDefault()
{
    m_length = 40;
    m_arrow_theta = PI / 9; // 20��
    m_arrow_length = 20;
    m_radius = 8;
}

void SimpleDirectionDraw::configExtraParams()
{

}

void SimpleDirectionDraw::updateExtraParams()
{

}

void SimpleDirectionDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

void SimpleDirectionDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void SimpleDirectionDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_length );
    reader.readDouble( m_arrow_theta );
    reader.readDouble( m_arrow_length );
    reader.readDouble( m_radius );
}

void SimpleDirectionDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_length );
    writer.writeDouble( m_arrow_theta );
    writer.writeDouble( m_arrow_length );
    writer.writeDouble( m_radius );
}

void SimpleDirectionDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "ͨ������" ) );
}

void SimpleDirectionDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    m_vt = _ttoi( values[0].kACharPtr() ) % 2; // ��ֵ֤Ϊ{0,1}
}

Adesk::Boolean SimpleDirectionDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    // ���Ƽ�ͷ����ֱ��
    DrawLine( mode, m_insertPt, m_angle, m_length );
    DrawLine( mode, m_insertPt, m_angle + PI, m_length );

    // ���Ƽ�ͷ����
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis );
    DrawLine( mode, m_insertPt + v * m_length, m_angle + PI - m_arrow_theta, m_arrow_length );
    DrawLine( mode, m_insertPt + v * m_length, m_angle - PI + m_arrow_theta, m_arrow_length );

    // �����۷���
    if( m_vt == 1 )
    {
        v.rotateBy( PI, AcGeVector3d::kZAxis );
        DrawSin( mode, m_insertPt + v * m_length, m_angle, m_radius );;
    }

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleDirectionDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    // �����任
    m_insertPt.transformBy( xform );

    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis );
    v.transformBy( xform );

    m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis ); // �任�����ת�Ƕ�

    return Acad::eOk;
}

Acad::ErrorStatus SimpleDirectionDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;
    // ��׽2�����͵ĵ㣺�˵�����ĵ�
    if( osnapMode != AcDb::kOsModeCen )
        return Acad::eOk;

    Acad::ErrorStatus es = Acad::eOk;

    if( osnapMode == AcDb::kOsModeCen )
    {
        snapPoints.append( m_insertPt );
    }

    return es;
}

Acad::ErrorStatus SimpleDirectionDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_insertPt );

    return Acad::eOk;
}

Acad::ErrorStatus SimpleDirectionDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        if ( idx == 0 )
        {
            m_insertPt += offset; 			// �����ƫ��
        }
    }
    return Acad::eOk;
}
