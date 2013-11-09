#include "StdAfx.h"
#include "DoubleWorkSurfaceDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

ACRX_CONS_DEFINE_MEMBERS ( DoubleWorkSurfaceDraw, DoubleTunnelDraw, 1 )

DoubleWorkSurfaceDraw::DoubleWorkSurfaceDraw( void )
{
}

void DoubleWorkSurfaceDraw::setAllExtraParamsToDefault()
{
    DoubleTunnelDraw::setAllExtraParamsToDefault();
    m_trunkWidth = 20;
    m_trunkLength = 70;
    m_arrowWidth = 15;
    m_arrowLength = 20;
    m_clockWise = false;
}

void DoubleWorkSurfaceDraw::configExtraParams()
{

}

void DoubleWorkSurfaceDraw::readExtraParam( DrawParamReader& reader )
{
    DoubleTunnelDraw::readExtraParam( reader );
    reader.readDouble( m_trunkWidth );
    reader.readDouble( m_trunkLength );
    reader.readDouble( m_arrowWidth );
    reader.readDouble( m_arrowLength );
    reader.readBoolean( m_clockWise );
}

void DoubleWorkSurfaceDraw::writeExtraParam( DrawParamWriter& writer )
{
    DoubleTunnelDraw::writeExtraParam( writer );
    writer.writeDouble( m_trunkWidth );
    writer.writeDouble( m_trunkLength );
    writer.writeDouble( m_arrowWidth );
    writer.writeDouble( m_arrowLength );
    writer.writeBoolean( m_clockWise );
}

// �����ͷ����ʼ�е�
static AcGePoint3d CaclArrowPoint( const AcGePoint3d& m_startPt, const AcGePoint3d& m_endPt, double m_width, double angle )
{
    AcGeVector3d v = m_endPt - m_startPt;
    AcGePoint3d pt = m_startPt + v * 0.5;
    v.normalize();
    v.rotateBy( angle, AcGeVector3d::kZAxis );
    return ( pt + v * m_width );
}

void DoubleWorkSurfaceDraw::drawArrow( AcGiWorldDraw* mode, bool clockWise )
{
    double angle = ( clockWise ? ( -PI / 2 ) : ( PI / 2 ) );

    // ��������ʼ������
    AcGePoint3d pt = CaclArrowPoint( m_startPt, m_endPt, m_width / 2, angle ); // �е�

    AcGeVector3d v = m_endPt - m_startPt;
    v.normalize();  // ������׼��(|v|=1)

    // ���������ʼ������
    AcGePoint3d sp1 = pt + v * m_trunkWidth;
    AcGePoint3d sp2 = pt - v * m_trunkWidth;

    v.rotateBy( angle, AcGeVector3d::kZAxis );

    // ���������ĩ������
    AcGePoint3d tp1 = sp1 + v * m_trunkLength;
    AcGePoint3d tp2 = sp2 + v * m_trunkLength;

    // ��ͷ�������
    AcGePoint3d ap = pt + v * ( m_trunkLength + m_arrowLength );

    v.rotateBy( -1 * angle, AcGeVector3d::kZAxis );

    // ��ͷ���������
    AcGePoint3d ap1 = tp1 + v * m_arrowWidth;
    AcGePoint3d ap2 = tp2 - v * m_arrowWidth;

    // ���Ƽ�ͷ
    DrawLine( mode, sp1, tp1 );
    DrawLine( mode, tp1, ap1 );
    DrawLine( mode, ap1, ap );
    DrawLine( mode, ap, ap2 );
    DrawLine( mode, ap2, tp2 );
    DrawLine( mode, tp2, sp2 );
}

void DoubleWorkSurfaceDraw::drawText( AcGiWorldDraw* mode )
{
    // ��������
    AcGeVector3d v = m_endPt - m_startPt;
    AcGePoint3d pt = m_startPt + v * 0.5; // ���ĵ�

    if( v.x < 0 ) v.negate();
    double angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    v.normalize();
    v.rotateBy( PI / 2, AcGeVector3d::kZAxis ); // ʼ�������ַ���
    pt += v * m_width * 0.5;

    DrawMText( mode, pt, angle, _T( "�زɹ�����" ), m_width * 0.618, AcDbMText::kBottomCenter );
}

Adesk::Boolean DoubleWorkSurfaceDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    //DoubleTunnelDraw::subWorldDraw(mode);
    DrawLine( mode, m_leftStartPt, m_leftEndPt );   // ��������
    DrawLine( mode, m_rightStartPt, m_rightEndPt );	// ��������
    DrawJoint( mode, m_startPt, m_width * 0.5, jdt ); // ����ʼ�ڵ�
    DrawJoint( mode, m_endPt, m_width * 0.5, jdt ); // ����ĩ�ڵ�

    // ��������
    drawText( mode );

    // ���ƻزɼ�ͷ
    drawArrow( mode, m_clockWise );

    return Adesk::kTrue;
}

Acad::ErrorStatus DoubleWorkSurfaceDraw::subGetGripPoints( AcGePoint3dArray& gripPoints,
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

        double angle = ( m_clockWise ? ( -PI / 2 ) : ( PI / 2 ) );
        // ��������ʼ������
        AcGePoint3d pt = CaclArrowPoint( m_startPt, m_endPt, m_width / 2, angle ); // �е�
        AcGeVector3d v = m_endPt - m_startPt;
        v.normalize();  // ������׼��(|v|=1)
        v.rotateBy( angle, AcGeVector3d::kZAxis );
        // ��ͷ�������
        AcGePoint3d ap = pt + v * ( m_trunkLength + m_arrowLength );

        gripPoints.append( ap );
    }

    return Acad::eOk;
}

Acad::ErrorStatus DoubleWorkSurfaceDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );

        if ( idx == 0 )
        {
            m_startPt += offset;
            updateExtraParams();
        }

        if ( idx == 1 )
        {
            m_endPt += offset;
            updateExtraParams();
        }

        if( idx == 2 )
        {
            double angle = ( m_clockWise ? ( -PI / 2 ) : ( PI / 2 ) );
            AcGeVector3d v = m_endPt - m_startPt;
            v.normalize();
            v.rotateBy( angle, AcGeVector3d::kZAxis );

            // ����offset��a1�����ϵ�ͶӰ����
            double L = offset.dotProduct( v );
            if( L < 0 && abs( L ) > ( m_trunkLength + m_arrowLength ) ) m_clockWise = !m_clockWise;
        }
    }
    return Acad::eOk;
}