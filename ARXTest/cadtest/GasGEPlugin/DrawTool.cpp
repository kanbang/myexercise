#include "StdAfx.h"

#include "DrawTool.h"

#define PI 3.1415926535897932384626433832795

AcGePoint2d Point3D_To_2D( const AcGePoint3d& pt )
{
    return AcGePoint2d( pt.x, pt.y );
}

AcGePoint3d Point2D_To_3D( const AcGePoint2d& pt )
{
    return AcGePoint3d( pt.x, pt.y, 0 );
}

CString MakeUpperText( const CString& inStr )
{
    CString str;
    str.Format( _T( "{\\H0.618x;\\S%s^;}" ), inStr );
    return str;
}

CString MakeLowerText( const CString& inStr )
{
    CString str;
    str.Format( _T( "{\\H0.618x;\\S^%s;}" ), inStr );
    return str;
}

/*
 * �ڻ��Ʊպ�ͼ��ʱ��AcGiFillTypeĬ��ΪkAcGiFillAlways (ʼ�����)
 * �պ�ͼ�ΰ�����Բ������Ρ������
 * �μ���AcGiSubEntityTraits::fillType()����˵��
 * ���磬����һ��Բ����ǰ��ɫ�Ǻڵװ�ɫ����ô�����Զ���ʵ����Ƶ�Բ��2�����:
 *	    1) arx������ص������-- �ױ�+�ڵ����(����Ч������cad��Բ��һ����)
 *		2) arx����ж�أ�cad���ô���ʵ��ģʽ��ʾͼԪ -- �ױ�+�׵����
 * ����μ����������Բ��һЩ˵��.doc
 */
void DrawCircle( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double radius, bool fill )
{
    AcGiSubEntityTraits& traits = mode->subEntityTraits();
    AcGiFillType ft = traits.fillType();
    traits.setFillType( fill ? kAcGiFillAlways : kAcGiFillNever );

    mode->geometry().circle( insertPt, radius, AcGeVector3d::kZAxis );

    // �ָ�����
    traits.setFillType( ft );
}

extern void DrawArc( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double radius, double startAngle, double sweepAngle, bool fill )
{
    AcGiSubEntityTraits& traits = mode->subEntityTraits();
    AcGiFillType ft = traits.fillType();
    traits.setFillType( fill ? kAcGiFillAlways : kAcGiFillNever );

    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( startAngle, AcGeVector3d::kZAxis );
    AcGiArcType arcType = ( fill ? kAcGiArcChord : kAcGiArcSimple );
    mode->geometry().circularArc( insertPt, radius, AcGeVector3d::kZAxis, v, sweepAngle, arcType );

    // �ָ�����
    traits.setFillType( ft );
}

void DrawArc( AcGiWorldDraw* mode, const AcGePoint3d& spt, const AcGePoint3d& pt, const AcGePoint3d& ept, bool fill )
{
    AcGiSubEntityTraits& traits = mode->subEntityTraits();
    AcGiFillType ft = traits.fillType();
    traits.setFillType( fill ? kAcGiFillAlways : kAcGiFillNever );

    AcGeCircArc3d arc( spt, pt, ept );
    AcGePoint3d cnt = arc.center();
    double radius = arc.radius();
    AcGeVector3d sv = spt - cnt;
    AcGeVector3d ev = ept - cnt;
    double sa = sv.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
    double ea = ev.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    if( arc.normal().z > 0 )
    {
        AcDbArc arcEnt( cnt, radius, sa, ea );
        arcEnt.worldDraw( mode );
    }
    else
    {
        AcDbArc arcEnt( cnt, radius, ea, sa );
        arcEnt.worldDraw( mode );
    }

    // �ָ�����
    traits.setFillType( ft );
}

void DrawLine( AcGiWorldDraw* mode, const AcGePoint3d& spt, const AcGePoint3d& ept )
{
    AcDbLine line( spt, ept );
    line.worldDraw( mode );
}

void DrawLine( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, double length )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );
    DrawLine( mode, pt, pt + v * length );
}

void DrawPolyLine( AcGiWorldDraw* mode, const AcGePoint3d& spt, const AcGePoint3d& ept, double width )
{
    AcDbPolyline pl( 2 );
    pl.addVertexAt( 0, Point3D_To_2D( spt ) );
    pl.addVertexAt( 1, Point3D_To_2D( ept ) );
    pl.setConstantWidth( width );

    pl.worldDraw( mode );
}

void DrawPolyLine( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, double length, double width )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );

    AcDbPolyline pl( 2 );
    pl.addVertexAt( 0, Point3D_To_2D( pt ) );
    pl.addVertexAt( 1, Point3D_To_2D( pt + v * length ) );
    pl.setConstantWidth( width );

    pl.worldDraw( mode );
}

void DrawMText( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, const CString& str, double height, AcDbMText::AttachmentPoint ap )
{
    //acutPrintf(_T("\n����ǰ--������ɫ:%d"), mode->subEntityTraits().color());
    AcDbMText mt;
    //AcDbObjectId style; // ������ʽ
    //pMText->setTextStyle(style);
    mt.setLocation( pt );
    mt.setTextHeight( height );
    mt.setAttachment( ap );
    mt.setRotation( angle );

    mt.setContents( str );

    // �������Է��֣�AcDbMText����worldDraw��ͬʱ���޸�mode��һЩ����
    mt.setColorIndex( mode->subEntityTraits().color() );

    mt.worldDraw( mode );

    //acutPrintf(_T("\n���ƺ�--������ɫ:%d"), mode->subEntityTraits().color());
}

static AcGePoint3d CaclPt( const AcGePoint3d& insertPt, const AcGeVector3d& v1, double width, const AcGeVector3d& v2, double height )
{
    return ( insertPt + v1 * width / 2 + v2 * height / 2 );
}

void DrawRect( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double angle, double width, double height, bool fill )
{
    AcGiSubEntityTraits& traits = mode->subEntityTraits();

    AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    v1.rotateBy( angle, AcGeVector3d::kZAxis );
    v2.rotateBy( angle + PI / 2, AcGeVector3d::kZAxis );

    AcGePoint3dArray pts;
    pts.append( CaclPt( insertPt, v1, width, v2, height ) );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( insertPt, v1, width, v2, height ) );

    v2.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( insertPt, v1, width, v2, height ) );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( insertPt, v1, width, v2, height ) );

    // �Ƿ����
    AcGiFillType ft = traits.fillType();
    traits.setFillType( fill ? kAcGiFillAlways : kAcGiFillNever );

    mode->geometry().polygon( pts.length(), pts.asArrayPtr() );

    traits.setFillType( ft );
}

void DrawSpline( AcGiWorldDraw* mode, const AcGePoint3dArray& pts )
{
    // ������������
    AcDbSpline sp( pts );
    sp.worldDraw( mode );
}

// ���Ƽ�ͷ
void DrawArrow( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double angle, double width, double length )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );

    AcDbPolyline pl( 2 );
    pl.addVertexAt( 0, Point3D_To_2D( insertPt ) );
    pl.addVertexAt( 1, Point3D_To_2D( insertPt + v * length ) );
    pl.setWidthsAt( 0, width, 0 );
    pl.setWidthsAt( 1, 0, 0 );

    pl.worldDraw( mode );
}
