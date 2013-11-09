#include "StdAfx.h"
#include "DoubleTTunnelDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

ACRX_CONS_DEFINE_MEMBERS ( DoubleTTunnelDraw, DoubleTunnelDraw, 1 )

DoubleTTunnelDraw::DoubleTTunnelDraw( void )
{
}

void DoubleTTunnelDraw::drawText( AcGiWorldDraw* mode )
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

Adesk::Boolean DoubleTTunnelDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    //DoubleTunnelDraw::subWorldDraw(mode);
    DrawLine( mode, m_leftStartPt, m_leftEndPt );   // ��������
    DrawLine( mode, m_rightStartPt, m_rightEndPt );	// ��������
    DrawJoint( mode, m_startPt, m_width * 0.5, jdt ); // ����ʼ�ڵ�
    DrawJoint( mode, m_endPt, m_width * 0.5, jdt ); // ����ĩ�ڵ�

    // ��������
    drawText( mode );

    return Adesk::kTrue;
}