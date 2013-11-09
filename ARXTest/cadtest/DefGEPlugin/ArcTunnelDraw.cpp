#include "StdAfx.h"
#include "ArcTunnelDraw.h"
#include <cmath>

// ʹ��ACRX_CONS_DEFINE_MEMBERS����������Ҫ
// ����Ҫdxf��dwg��ĺ�
// �汾�����дһ������
ACRX_CONS_DEFINE_MEMBERS ( ArcTunnelDraw, ConnectedGEDraw, 1 )

ArcTunnelDraw::ArcTunnelDraw()
{
}

ArcTunnelDraw::~ArcTunnelDraw( void )
{
}

void ArcTunnelDraw::setAllParamsToDefault()
{
    m_arcRatio = 0.15;
}

void ArcTunnelDraw::config()
{

}

void ArcTunnelDraw::readExtraParam( GEDrawParamReader& reader )
{
    reader.readDouble( m_arcRatio );
}

void ArcTunnelDraw::writeExtraParam( GEDrawParamWriter& writer )
{
    writer.writeDouble( m_arcRatio );
}

AcGePoint3d ArcTunnelDraw::caclArcCenterPt() const
{
    AcGeVector3d v = m_endPt - m_startPt;

    // �ҵ��е�����
    AcGePoint3d pt = m_startPt + v / 2;

    // �����ҵ��е㵽�����е�ľ���
    // ��ʽ: d = �ҳ�/2 * ( ƽ����( 4*���ʵ�ƽ�� + 1 ) - 2*���� )
    double d = v.length() * 0.5 * ( sqrt( 4 * m_arcRatio * m_arcRatio + 1 ) - 2 * m_arcRatio );

    v.rotateBy( PI / 2, AcGeVector3d::kZAxis ); // ��ת90��(��ֱ����)
    v.normalize(); // ��vת���ɵ�λ����
    v *= d;

    // �������е�����
    return pt + v;
}

Adesk::Boolean ArcTunnelDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    mode->geometry().circularArc( m_startPt, caclArcCenterPt(), m_endPt );

    return Adesk::kTrue;
}

Acad::ErrorStatus ArcTunnelDraw::subGetOsnapPoints( AcDb::OsnapMode osnapMode, int gsSelectionMark, const AcGePoint3d& pickPoint, const AcGePoint3d& lastPoint, const AcGeMatrix3d& viewXform, AcGePoint3dArray& snapPoints, AcDbIntArray& geomIds ) const
{
    assertReadEnabled();

    // ִ�и���Ĳ�׽��Ϊ
    Acad::ErrorStatus es =
        ConnectedGEDraw::subGetOsnapPoints( osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds );

    if( es != Acad::eOk ) return es;

    // ���Ӳ�׽1�����͵ĵ㣺�е�
    if( osnapMode != AcDb::kOsMaskMid ) return Acad::eOk;

    if( osnapMode ==  AcDb::kOsModeMid )
    {
        snapPoints.append( caclArcCenterPt() ); // �����е�Ҳ��Ϊ��׽��
    }

    return Acad::eOk;
}

Acad::ErrorStatus ArcTunnelDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    Acad::ErrorStatus es =
        ConnectedGEDraw::subGetGripPoints( gripPoints, osnapModes, geomIds );

    if( es != Acad::eOk ) return es;

    gripPoints.append( caclArcCenterPt() ); // �����е�Ҳ��Ϊ�е�

    return Acad::eOk;
}

Acad::ErrorStatus ArcTunnelDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    Acad::ErrorStatus es =
        ConnectedGEDraw::subMoveGripPointsAt( indices, offset );

    if( es != Acad::eOk ) return es;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        // �����е�
        if ( idx == 2 )
        {
            AcGePoint3d centerPt = caclArcCenterPt();
            centerPt += offset;

            AcGeCircArc3d arc( m_startPt, centerPt, m_endPt );
            AcGePoint3d pt = arc.center(); // Բ������
            AcGeLine3d line( m_startPt, m_endPt );

            // ����Բ�ĵ��ҵľ���
            double d = line.distanceTo( pt );

            // �����ҵĳ���
            double l = m_startPt.distanceTo( m_endPt );

            // ���»���
            m_arcRatio = d / l;
        }
    }
    return ( Acad::eOk );
}