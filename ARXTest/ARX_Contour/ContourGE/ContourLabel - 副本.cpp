#include "StdAfx.h"
#include "ContourLabel.h"

/* ȫ�ֺ���(ARX_Smooth.cpp) */
extern void ARX_SmoothPoints( const AcGePoint3dArray& inPts, AcGePoint3dArray& outPts );
/* ȫ�ֺ���(ARX_Label.cpp) */
extern void ARX_Label( const AcGePoint3dArray& pts, AcGePoint3d& tpt, double& angle );

static void DrawLinePoints( AcGiWorldDraw* mode, const AcGePoint3dArray& pts )
{
    int len = pts.length();
    for( int i = 0; i < len - 1; i++ )
    {
        AcGePoint3dArray tpts;
        tpts.append( pts[i] );
        tpts.append( pts[i + 1] );
        mode->geometry().worldLine( tpts.asArrayPtr() );
    }
}

static void GetTextAndWidth( double z, double height, CString& text, double& width )
{
    AcDbMText mt;
    mt.setLocation( AcGePoint3d::kOrigin );
    mt.setRotation( 0 );
    mt.setTextHeight( height );
    mt.setAttachment( AcDbMText::kMiddleCenter ); // Ĭ�Ͼ���

    // ��ȡ��������
    text.Format( _T( " %.3f " ), z );
    mt.setContents( text );

    // ��ȡ���ֿ��
    width = mt.actualWidth();
}

static void DrawZValue( AcGiWorldDraw* mode, double z, const AcGePoint3d& tpt, double angle, double height )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );
    if( v.x < 0 )
    {
        angle += PI;
    }

    CString text;
    text.Format( _T( " %.5f " ), z );

    AcDbMText mt;
    mt.setLocation( tpt );
    mt.setTextHeight( height );
    mt.setAttachment( AcDbMText::kMiddleCenter );
    mt.setRotation( angle );
    mt.setBackgroundFill( true );
    mt.setUseBackgroundColor( true );
    mt.setBackgroundScaleFactor( 1.0 );
    mt.setContents( text );

    // �������Է��֣�AcDbMText����worldDraw��ͬʱ���޸�mode��һЩ����
    mt.setColorIndex( mode->subEntityTraits().color() );

    mt.worldDraw( mode );
}

static void DrawSmoothPoints( AcGiWorldDraw* mode, const AcGePoint3dArray& pts )
{
    // ʹ�������������й⻬
    AcGePoint3dArray spline_pts;
    ARX_SmoothPoints( pts, spline_pts );

    DrawLinePoints( mode, spline_pts );
}

Adesk::UInt32 ContourLabel::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    ContourLabel, ContourGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ��ֵ�߱�ע, CONTOURAPP )

ContourLabel::ContourLabel () : ContourGE (), m_bSmooth( true ), m_textHeight( 1.2 )
{
}

ContourLabel::~ContourLabel ()
{
}

Acad::ErrorStatus ContourLabel::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = ContourGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( ContourLabel::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    // д��zֵ
    pFiler->writeDouble( m_zValue );

    // д���������
    int len = m_pts.length();
    pFiler->writeInt32( len );

    // д������
    for( int i = 0; i < len; i++ )
    {
        pFiler->writePoint3d( m_pts[i] );
    }

    // д���עλ��
    pFiler->writePoint3d( m_tpt );

    // д���ע����
    pFiler->writeDouble( m_angle );

    // д���ע���ִ�С
    pFiler->writeDouble( m_textHeight );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus ContourLabel::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = ContourGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > ContourLabel::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    // ��ȡzֵ
    pFiler->readDouble( &m_zValue );

    // ��ȡ�������
    long len = 0;
    pFiler->readInt32( &len );

    // ��ȡ����
    m_pts.removeAll();
    for( int i = 0; i < ( int )len; i++ )
    {
        AcGePoint3d pt;
        pFiler->readPoint3d( &pt );
        m_pts.append( pt );
    }

    // ��ȡ��עλ��
    pFiler->readPoint3d( &m_tpt );

    // ��ȡ��ע����
    pFiler->readDouble( &m_angle );

    // ��ȡ��ע���ָ߶�
    pFiler->readDouble( &m_textHeight );

    return ( pFiler->filerStatus() ) ;
}

Adesk::Boolean ContourLabel::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    // ���Ƶ�ֵ��
    if( m_bSmooth )
    {
        DrawSmoothPoints( mode, m_pts );
    }
    else
    {
        DrawLinePoints( mode, m_pts );
    }

    // ��ע����
    DrawZValue( mode, m_zValue, m_tpt, m_angle, m_textHeight );

    return Adesk::kTrue;
}

static AcGePoint3d FindClosePoint( const AcGePoint3dArray& pts, const AcGePoint3d& pt )
{
    AcGePolyline3d pline( pts );

    AcGePoint3d cpt( pt );
    return pline.closestPointTo( pt );
}

void ContourLabel::smooth( bool bSmooth )
{
    assertWriteEnabled();
    this->m_bSmooth = bSmooth;

    ARX_Label( m_pts, m_tpt, m_angle );

    if( m_bSmooth )
    {
        AcGePoint3dArray pts;
        ARX_SmoothPoints( m_pts, pts );
        m_tpt = FindClosePoint( pts, m_tpt );
    }
}

void ContourLabel::setTextHeight( double height )
{
    assertWriteEnabled();
    this->m_textHeight = height;
}

double ContourLabel::getZValue() const
{
    assertReadEnabled();
    return m_zValue;
}

void ContourLabel::setZValues( double z )
{
    assertWriteEnabled();
    this->m_zValue = z;
}

void ContourLabel::getPoints( AcGePoint3dArray& cnpts ) const
{
    assertReadEnabled();
    cnpts.append( m_pts );
}

void ContourLabel::setPoints( const AcGePoint3dArray& cnpts )
{
    assertWriteEnabled();
    if( cnpts.length() > 2 )
    {
        m_pts.removeAll();
        m_pts.append( cnpts );
    }
}