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
    bc.setRGB( 0, 0, 0 ); // ʹ�ú�ɫ��Ϊ��2����ɫ
    DrawCircleGradient( mode, pt, radius, ( "INVSPHERICAL" ), c, bc ); // // ��ת����
}

Adesk::Boolean SphereNitrogenPipeDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    // ������ɫ�������
    AcCmColor c;
    c.setRGB( 0, 255, 0 );
    DrawSphereNitrogenPipe( mode, m_pt, m_radius, c );

    // ����Բ
    DrawCircle( mode, m_pt, m_radius, false );

    return Adesk::kTrue;
}

Acad::ErrorStatus SphereNitrogenPipeDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    // �����任
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
    // ��׽2�����͵ĵ㣺�˵�����ĵ�
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
            m_pt += offset; 			// �����ƫ��
        }
    }
    return Acad::eOk;
}
