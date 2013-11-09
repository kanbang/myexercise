#include "StdAfx.h"
#include "SimpleGateDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleGateDraw, MineGEDraw, 1 )

SimpleGateDraw::SimpleGateDraw () : MineGEDraw (), m_gap( 0 ), m_offset( 0 )
{
}

SimpleGateDraw::~SimpleGateDraw ()
{
}

void SimpleGateDraw::setAllExtraParamsToDefault()
{
    m_radius = 40;
    m_length = 50;
    m_dTWidth = 40; // ��˫������Ŀ������ͬ�ģ�˫�������ȵ�����ͬʱҲӦ��Ҫ�޸ĸÿ��ֵ
}

void SimpleGateDraw::configExtraParams()
{

}

void SimpleGateDraw::updateExtraParams()
{

}

void SimpleGateDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

void SimpleGateDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void SimpleGateDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_radius );
    reader.readDouble( m_length );
    reader.readDouble( m_dTWidth );
}

void SimpleGateDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_radius );
    writer.writeDouble( m_length );
    writer.writeDouble( m_dTWidth );
}

void SimpleGateDraw::drawArc( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double angle, double gap, double radius, bool clockWise )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle + PI, AcGeVector3d::kZAxis );
    AcGePoint3d pt = insertPt + v * gap; // �����λ��ƫ��

    AcGeVector3d endVector( AcGeVector3d::kXAxis );
    if( clockWise )
    {
        endVector.rotateBy( angle + PI / 2, AcGeVector3d::kZAxis );
    }
    else
    {
        endVector.rotateBy( angle - PI / 2, AcGeVector3d::kZAxis );
    }
    mode->geometry().circularArc( pt, radius, AcGeVector3d::kZAxis, endVector, PI ); // ����һ����Բ
}

void SimpleGateDraw::drawLine( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double angle, double gap, double offset, double length )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle + PI, AcGeVector3d::kZAxis );
    AcGePoint3d pt = insertPt + v * gap; // �����λ��ƫ��

    AcGeVector3d startVector( AcGeVector3d::kXAxis ), endVector( AcGeVector3d::kXAxis );

    // ע�⣺rotateBy����ͬʱ���޸Ķ�Ӧ��this����
    startVector.rotateBy( angle - PI / 2, AcGeVector3d::kZAxis );
    endVector.rotateBy( angle + PI / 2, AcGeVector3d::kZAxis );

    DrawLine( mode, pt + startVector * offset, pt + startVector * length );
    DrawLine( mode, pt + endVector * offset, pt + endVector * length );
}

Adesk::Boolean SimpleGateDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    drawArc( mode, m_insertPt, m_angle, m_gap, m_radius ); // ���ư�Բ
    drawLine( mode, m_insertPt, m_angle, m_gap, m_offset, m_length ); // ���������ֱ��

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleGateDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_insertPt.transformBy( xform ); // �仯�����

    AcGeVector3d v( AcGeVector3d::kXAxis * m_radius );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis );
    v.transformBy( xform );

    m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis ); // �任�����ת�Ƕ�
    //m_radius = v.length(); // �仯��İ뾶

    /*AcGeVector3d startVector(AcGeVector3d::kXAxis*m_length);
    startVector.rotateBy(PI/2+m_angle, AcGeVector3d::kZAxis);
    startVector.transformBy(xform);*/

    //m_length = startVector.length(); // �仯��ĳ���

    return Acad::eOk;
}

//- Osnap points protocol
Acad::ErrorStatus SimpleGateDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // ֻ��׽1�����͵ĵ㣺�е�
    if( osnapMode != AcDb::kOsModeCen ) return Acad::eOk;

    if( osnapMode == AcDb::kOsModeCen )
    {
        snapPoints.append( m_insertPt );
    }

    return Acad::eOk;
}

//- Grip points protocol
Acad::ErrorStatus SimpleGateDraw::subGetGripPoints ( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // �������Ϊ�е�
    gripPoints.append( m_insertPt );

    // Բ���ĵ�һ������
    /*AcGeVector3d v(AcGeVector3d::kXAxis*(m_radius+m_gap));
    v.rotateBy(m_angle+PI, AcGeVector3d::kZAxis);
    gripPoints.append(m_insertPt+v);*/

    // ֱ�ߵ�2���˵�
    /*AcGeVector3d startVector(AcGeVector3d::kXAxis*m_length),
    	         endVector(AcGeVector3d::kXAxis*m_length);
    startVector.rotateBy(m_angle-PI/2, AcGeVector3d::kZAxis);
    endVector.rotateBy(m_angle+PI/2, AcGeVector3d::kZAxis);

    gripPoints.append(m_insertPt + startVector);
    gripPoints.append(m_insertPt + endVector);*/

    return Acad::eOk;
}

Acad::ErrorStatus SimpleGateDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        // �����
        if ( idx == 0 ) m_insertPt += offset;

        // Բ������
        /*if (idx == 1)
        {*/
        // ����Բ���뾶
        //AcGeVector3d v(AcGeVector3d::kXAxis*m_radius);
        //v.rotateBy(m_angle+PI, AcGeVector3d::kZAxis);
        //v += offset; // �뾶��������ƫ��

        //m_radius = v.length(); // ���°뾶
        //m_angle = v.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis); // �任�����ת�Ƕ�
        //}

        //if(idx == 2)
        //{
        //	AcGeVector3d startVector(AcGeVector3d::kXAxis*m_length);

        //	// ע�⣺rotateBy����ͬʱ���޸Ķ�Ӧ��this����
        //	startVector.rotateBy(m_angle-PI/2, AcGeVector3d::kZAxis);
        //	startVector += offset; // �˵���������ƫ��

        //	// ��ʱ������ֱ�߳���
        //	//m_length = startVector.length()*2; // ����ֱ�߳���

        //	startVector.rotateBy(PI/2, AcGeVector3d::kZAxis); // ˳ʱ����ת90��
        //	m_angle = startVector.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis); // �任�����ת�Ƕ�
        //}

        //if(idx == 3)
        //{
        //	AcGeVector3d endVector(AcGeVector3d::kXAxis*m_length);
        //	endVector.rotateBy(PI/2+m_angle, AcGeVector3d::kZAxis);
        //	endVector += offset; // �˵���������ƫ��

        //	// ��ʱ������ֱ�߳���
        //	//m_length = endVector.length()*2; // ����ֱ�߳���

        //	endVector.rotateBy(-PI/2, AcGeVector3d::kZAxis); // ��ʱ����ת90��
        //	m_angle = endVector.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis); // �任�����ת�Ƕ�
        //}
    }
    return Acad::eOk;
}

static AcGePoint3d CaclPt( const AcGePoint3d& insertPt, const AcGeVector3d& v1, double width, const AcGeVector3d& v2, double height )
{
    return ( insertPt + v1 * width + v2 * height );
}

static double CaclWidth( double w, double dw )
{
    double width = w;
    if( w > dw / 2 )
    {
        width = sqrt( w * w - dw * dw / 4 );
    }
    return width;
}

void SimpleGateDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    v1.rotateBy( m_angle + PI / 2, AcGeVector3d::kZAxis );
    v2.rotateBy( m_angle + PI, AcGeVector3d::kZAxis );

    double width = CaclWidth( m_radius, m_dTWidth );

    pts.append( m_insertPt + v1 * m_length );
    pts.append( CaclPt( m_insertPt, v1, m_length, v2, width ) );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( m_insertPt, v1, m_length, v2, width ) );
    pts.append( m_insertPt + v1 * m_length );
}