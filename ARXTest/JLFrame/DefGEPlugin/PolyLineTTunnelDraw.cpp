#include "StdAfx.h"
#include "PolyLineTTunnelDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

ACRX_CONS_DEFINE_MEMBERS ( PolyLineTTunnelDraw, PolyLineTunnelDraw, 1 )

PolyLineTTunnelDraw::PolyLineTTunnelDraw( void )
{
}

void PolyLineTTunnelDraw::drawText( AcGiWorldDraw* mode )
{
    // ��������
    AcGeVector3d v = m_endPt - m_startPt;
    AcGePoint3d pt = m_startPt + v * 0.5; // ���ĵ�

    if( v.x < 0 ) v.negate();
    double angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    v.normalize();
    v.rotateBy( PI / 2, AcGeVector3d::kZAxis ); // ʼ�������ַ���

    pt += v * m_width * 0.5;

    DrawMText( mode, pt, angle, _T( "���������" ), m_width * 0.618, AcDbMText::kBottomCenter );
}

Adesk::Boolean PolyLineTTunnelDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    //PolyLineTunnelDraw::subWorldDraw(mode);
    DrawPolyLine( mode, m_startPt, m_endPt, m_width );
    DrawJoint( mode, m_startPt, m_width * 0.5, jdt ); // ����ʼ�ڵ�
    DrawJoint( mode, m_endPt, m_width * 0.5, jdt ); // ����ĩ�ڵ�

    // ����һ������
    drawText( mode );

    return Adesk::kTrue;
}
