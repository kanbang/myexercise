#include "StdAfx.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

#define PI 3.1415926535897932384626433832795

void DrawCross( AcGiWorldDraw* mode, const AcGePoint3d& pt, double radius )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v *= radius;

    for( int i = 0; i < 4; i++ )
    {
        AcGePoint3dArray pts;
        pts.append( pt );
        pts.append( pt + v );
        mode->geometry().worldLine( pts.asArrayPtr() );

        v.rotateBy( PI / 2, AcGeVector3d::kZAxis );
    }
}

// ��������(2����Բ)
void DrawSin( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, double radius )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );

    // ���Ƶ�1����Բ
    mode->geometry().circularArc( pt + v * radius, radius, AcGeVector3d::kZAxis, v, PI );

    // ���Ƶ�2����Բ
    // ע����ʹ��v.negate()����, �÷������޸�v
    mode->geometry().circularArc( pt + v * radius * 3, radius, AcGeVector3d::kZAxis, -v, PI );
}

void DrawShaft( AcGiWorldDraw* mode, const AcGePoint3d& pt, double radius )
{
    // �����⻷
    DrawCircle( mode, pt, radius, false );

    // �����ڻ�
    DrawArc( mode, pt, radius * 0.8, PI / 6, PI, true );
    DrawArc( mode, pt, radius * 0.8, PI * 7 / 6, PI, false );
}

void DrawJoint( AcGiWorldDraw* mode, const AcGePoint3d& pt, double radius, JointDrawType jdt )
{
    if( jdt == JDT_CROSS )
    {
        DrawCircle( mode, pt, radius, false );
        DrawCross( mode, pt, radius );
    }
    else if( jdt == JDT_FILL )
    {
        DrawCircle( mode, pt, radius, true );
    }
}