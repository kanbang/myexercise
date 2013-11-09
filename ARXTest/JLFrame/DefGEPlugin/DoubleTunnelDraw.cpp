#include "StdAfx.h"
#include "DoubleTunnelDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"
#include <cmath>

ACRX_CONS_DEFINE_MEMBERS ( DoubleTunnelDraw, LinkedGEDraw, 1 )

DoubleTunnelDraw::DoubleTunnelDraw()
{
}

void DoubleTunnelDraw::setAllExtraParamsToDefault()
{
    m_width = 40;
    jdt = JDT_NULL;
}

void DoubleTunnelDraw::configExtraParams()
{
    // �л���Դ
    //CAcModuleResourceOverride myResources;
}

void DoubleTunnelDraw::updateExtraParams()
{
    update();
}

void DoubleTunnelDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_width );
    reader.readPoint( m_leftStartPt );
    reader.readPoint( m_leftEndPt );
    reader.readPoint( m_rightStartPt );
    reader.readPoint( m_rightEndPt );

    int i = 0;
    reader.readInt( i );
    jdt = ( JointDrawType )( i % 3 );
}

void DoubleTunnelDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_width );
    writer.writePoint( m_leftStartPt );
    writer.writePoint( m_leftEndPt );
    writer.writePoint( m_rightStartPt );
    writer.writePoint( m_rightEndPt );
    writer.writeInt( ( int )jdt ); // ת��������
}

void DoubleTunnelDraw::update()
{
    caclStartPoint( m_leftStartPt, m_rightStartPt );
    caclEndPoint( m_leftEndPt, m_rightEndPt );
}

void DoubleTunnelDraw::caclStartPoint( AcGePoint3d& startPt1, AcGePoint3d& startPt2 )
{
    AcGeVector3d v = m_endPt - m_startPt;
    v.normalize();

    v.rotateBy( PI * 0.5, AcGeVector3d::kZAxis );
    startPt1 = m_startPt + v * m_width * 0.5;

    v.rotateBy( PI, AcGeVector3d::kZAxis );
    startPt2 = m_startPt + v * m_width * 0.5;
}

void DoubleTunnelDraw::caclEndPoint( AcGePoint3d& endPt1, AcGePoint3d& endPt2 )
{
    AcGeVector3d v = m_endPt - m_startPt;
    v.normalize();

    v.rotateBy( PI * 0.5, AcGeVector3d::kZAxis );
    endPt1 = m_endPt + v * m_width * 0.5;

    v.rotateBy( PI, AcGeVector3d::kZAxis );
    endPt2 = m_endPt + v * m_width * 0.5;
}

void DoubleTunnelDraw::dealWithStartPointBoundary( const AcGeRay3d& boundaryLine )
{
    AcGeLine3d line( m_leftStartPt, m_leftEndPt );

    AcGePoint3d pt;
    if( Adesk::kTrue == line.intersectWith( boundaryLine, pt ) ) // ���������������߽��ߵĽ����
        m_leftStartPt = pt;                        // ������������ߵ�ʼ������

    line.set( m_rightStartPt, m_rightEndPt );
    if( Adesk::kTrue == line.intersectWith( boundaryLine, pt ) ) // �����Ҳ���������߽��ߵĽ����
        m_rightStartPt = pt;                       // �����Ҳ������ߵ�ʼ������
}

void DoubleTunnelDraw::dealWithEndPointBoundary( const AcGeRay3d& boundaryLine )
{
    AcGeLine3d line( m_leftStartPt, m_leftEndPt );

    AcGePoint3d pt;
    if( Adesk::kTrue == line.intersectWith( boundaryLine, pt ) ) // ���������������߽��ߵĽ����
        m_leftEndPt = pt;                                         // ������������ߵ�ĩ������

    line.set( m_rightStartPt, m_rightEndPt );
    if( Adesk::kTrue == line.intersectWith( boundaryLine, pt ) ) // �����Ҳ���������߽��ߵĽ����
        m_rightEndPt = pt;                          // �����Ҳ������ߵ�ĩ������
}

static void SwapPoint( AcGePoint3d& pt1, AcGePoint3d& pt2 )
{
    AcGePoint3d tpt = pt1;
    pt1 = pt2;
    pt2 = tpt;
}

void DoubleTunnelDraw::reverse()
{
    LinkedGEDraw::reverse();
    SwapPoint( m_leftStartPt, m_leftEndPt );
    SwapPoint( m_rightStartPt, m_rightEndPt );
}

void DoubleTunnelDraw::extendByLength( double length )
{
    AcGeVector3d v = m_endPt - m_startPt;
    v.normalize();

    m_endPt = m_endPt + v * length; // ����ĩ������
    update(); // ������������
}

void DoubleTunnelDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    pts.append( m_startPt );

    pts.append( m_leftStartPt );
    pts.append( m_leftEndPt );

    pts.append( m_endPt );

    pts.append( m_rightEndPt );
    pts.append( m_rightStartPt );
}

Adesk::Boolean DoubleTunnelDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    double c = 0.618;
    DrawLine( mode, m_leftStartPt, m_leftEndPt );   // ��������
    DrawLine( mode, m_rightStartPt, m_rightEndPt );	// ��������
    //DrawJoint(mode, m_startPt, m_width*0.5*c, jdt);  // ����ʼ�ڵ�
    //DrawJoint(mode, m_endPt, m_width*0.5*c, jdt);    // ����ĩ�ڵ�

    return Adesk::kTrue;
}

Acad::ErrorStatus DoubleTunnelDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_startPt.transformBy( xform );
    m_endPt.transformBy( xform );
    updateExtraParams();

    return Acad::eOk;
}

Acad::ErrorStatus DoubleTunnelDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // ֻ��׽�˵�
    if ( osnapMode == AcDb::kOsModeEnd )
    {
        snapPoints.append( m_startPt );
        snapPoints.append( m_endPt );
        //snapPoints.append(m_leftStartPt);
        //snapPoints.append(m_leftEndPt);
        //snapPoints.append(m_rightStartPt);
        //snapPoints.append(m_rightEndPt);
    }

    return Acad::eOk;
}

Acad::ErrorStatus DoubleTunnelDraw::subGetGripPoints ( AcGePoint3dArray& gripPoints,
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
    }

    return Acad::eOk;
}

Acad::ErrorStatus DoubleTunnelDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );

        if ( idx == 0 )
        {
            /*if(m_startPt == m_endPt)
            {
            	m_startPt += offset;
            	m_endPt += offset;
            }
            else
            {*/
            m_startPt += offset;
            //}
            updateExtraParams();
        }

        if ( idx == 1 )
        {
            //if(m_startPt == m_endPt) // ʼĩ���غ�
            //{
            //	AcGePoint3d pt(m_startPt);
            //	pt.x = pt.x + m_width*0.3; // �⻷�뾶��60%
            //	pt += offset;
            //	m_endPt = pt;
            //}
            //else
            //{
            m_endPt += offset;
            //}
            updateExtraParams();
        }
    }
    return Acad::eOk;
}