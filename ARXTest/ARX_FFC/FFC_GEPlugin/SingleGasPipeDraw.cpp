#include "StdAfx.h"
#include "SingleGasPipeDraw.h"

#include "DrawTool.h"
#include <cmath>

ACRX_CONS_DEFINE_MEMBERS ( SingleGasPipeDraw, MineGEDraw, 1 )

SingleGasPipeDraw::SingleGasPipeDraw()
{
}

void SingleGasPipeDraw::setAllExtraParamsToDefault()
{
    m_radius = FFC_RADIUS_COEFF * FFC_TUNNEL_WIDTH;
}

void SingleGasPipeDraw::configExtraParams()
{
    // �л���Դ
    //CAcModuleResourceOverride myResources;
}

void SingleGasPipeDraw::updateExtraParams()
{
}

void SingleGasPipeDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_startPt );
    reader.readPoint( m_endPt );
}

void SingleGasPipeDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_startPt );
    writer.writePoint( m_endPt );
}

void SingleGasPipeDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_radius );
}

void SingleGasPipeDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_radius );
}

static void DrawSphereGasPipe( AcGiWorldDraw* mode, const AcGePoint3d& pt, double radius, const AcCmColor& c )
{
    AcCmColor bc;
    bc.setRGB( 0, 0, 0 ); // ʹ�ú�ɫ��Ϊ��2����ɫ
    DrawCircleGradient( mode, pt, radius, ( "INVSPHERICAL" ), c, bc ); // // ��ת����
}

Adesk::Boolean SingleGasPipeDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    DrawLine( mode, m_startPt, m_endPt );

    // ������ɫ�������
    AcCmColor color;
    color.setRGB( 0, 0, 255 );
    DrawSphereGasPipe( mode, m_endPt, m_radius, color );

    // ����Բ
    DrawCircle( mode, m_endPt, m_radius, false );

    return Adesk::kTrue;
}

Acad::ErrorStatus SingleGasPipeDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_startPt.transformBy( xform );
    m_endPt.transformBy( xform );
    updateExtraParams();

    return Acad::eOk;
}

Acad::ErrorStatus SingleGasPipeDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // ֻ��׽1�����͵ĵ㣺�˵�
    if( osnapMode != AcDb::kOsModeEnd )
        return Acad::eOk;

    Acad::ErrorStatus es = Acad::eOk;

    // �����Ҫ�趨��׽��λ�ã����ܻ���Ҫ����һЩ��׽��
    // �����е㡢2/3����ȵ�
    if ( osnapMode == AcDb::kOsModeEnd )
    {
        snapPoints.append( m_startPt );
        snapPoints.append( m_endPt );
    }

    return es;
}

//- Grip points protocol
Acad::ErrorStatus SingleGasPipeDraw::subGetGripPoints ( AcGePoint3dArray& gripPoints,
        AcDbIntArray& osnapModes,
        AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_startPt );
    gripPoints.append( m_endPt );

    return Acad::eOk;
}

Acad::ErrorStatus SingleGasPipeDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
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

    //updateExtraParams();

    return Acad::eOk;
}