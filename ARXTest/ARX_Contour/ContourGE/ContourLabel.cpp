#include "StdAfx.h"
#include "ContourLabel.h"
#include <vector>
#include <algorithm>
#include <cfloat>

/* 全局函数(ARX_Smooth.cpp) */
extern void ARX_SmoothPoints( const AcGePoint3dArray& inPts, AcGePoint3dArray& outPts );

static void GetTextAndWidth( double z, double height, CString& text, double& width )
{
    AcDbMText mt;
    mt.setLocation( AcGePoint3d::kOrigin );
    mt.setRotation( 0 );
    mt.setTextHeight( height );
    mt.setAttachment( AcDbMText::kMiddleCenter ); // 默认居中

    // 获取文字内容
    text.Format( _T( " %.4f " ), z );
    mt.setContents( text );

    // 获取文字宽度
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
    text.Format( _T( " %.4f " ), z );

    AcDbMText mt;
    mt.setLocation( tpt );
    mt.setTextHeight( height );
    mt.setAttachment( AcDbMText::kMiddleCenter );
    mt.setRotation( angle );
    mt.setBackgroundFill( true );
    mt.setUseBackgroundColor( true );
    mt.setBackgroundScaleFactor( 1.0 );
    mt.setContents( text );

    // 经过测试发现，AcDbMText调用worldDraw的同时会修改mode的一些属性
    mt.setColorIndex( mode->subEntityTraits().color() );

    mt.worldDraw( mode );
}

static bool IsPointOnline( const AcGePoint3d& p1, const AcGePoint3d& p2, const AcGePoint3d& p )
{
    AcGeLineSeg3d line( p1, p2 );
    return ( Adesk::kTrue == line.isOn( p ) );
}

static AcGePoint3d MidPoint( const AcGePoint3d& p1, const AcGePoint3d& p2 )
{
    AcGeVector3d v = p2 - p1;
    return p1 + v * 0.5;
}

static double CaclAngle( const AcGePoint3d& p1, const AcGePoint3d& p2 )
{
    AcGeVector3d v = p2 - p1;
    return v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
}

static int FindPointOnPLine( const AcGePoint3dArray& pts, const AcGePoint3d& pt )
{
    int pos = -1;
    int n = pts.length();
    for( int i = 0; i < n - 1; i++ )
    {
        AcGePoint3d p1 = pts[i];
        AcGePoint3d p2 = pts[i + 1];
        if( p1 == pt )
        {
            pos = i;
            break;
        }
        else if( IsPointOnline( p1, p2, pt ) )
        {
            pos = i;
            break;
        }
    }

    return pos;
}

static AcGePoint3d FindClosePoint( const AcGePoint3dArray& pts, const AcGePoint3d& pt )
{
    /*AcGePolyline3d pline(pts);
    return pline.closestPointTo(pt);*/

    AcGePoint3d cpt = pt;
    double cpt_dist = DBL_MAX;

    int n = pts.length();
    for( int i = 0; i < n - 1; i++ )
    {
        AcGePoint3d p1 = pts[i], p2 = pts[i + 1];

        if( pt == p1 )
        {
            cpt = MidPoint( p1, p2 );
            break;
        }
        else
        {
            AcGeLineSeg3d line( p1, p2 );
            AcGePoint3d pn = line.closestPointTo( pt );
            if( pn == p1 || pn == p2 ) continue;

            double dist = pn.distanceTo( pt );
            if( dist < cpt_dist )
            {
                cpt_dist = dist;
                cpt = pn;
            }
        }
    }

    return cpt;
}

static bool AdjustPoint( const AcGePoint3dArray& pts, AcGePoint3d& pt, double& angle )
{
    //acutPrintf(_T("\n(%.3f, %.3f)"), pt.x, pt.y);
    pt = FindClosePoint( pts, pt );
    //acutPrintf(_T("\n(%.3f, %.3f)"), pt.x, pt.y);

    int n = pts.length();
    int pos = FindPointOnPLine( pts, pt );
    //acutPrintf(_T("\npos=%d"), pos);
    if( pos == -1 )
    {
        return false;
    }
    else if( pos == 0 )
    {
        pt = MidPoint( pts[pos], pts[pos + 1] );
        angle = CaclAngle( pts[pos], pts[pos + 1] );
    }
    else if( pos == n - 1 )
    {
        pt = MidPoint( pts[pos - 1], pts[pos] );
        angle = CaclAngle( pts[pos - 1], pts[pos] );
    }
    else
    {
        angle = CaclAngle( pts[pos], pts[pos + 1] );
    }
    return true;
}

Adesk::UInt32 ContourLabel::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    ContourLabel, ContourGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    等值线标注, CONTOURAPP )

ContourLabel::ContourLabel () : ContourGE (), m_bSmooth( true ), m_textHeight( 1.2 ), m_angle( 0 )
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

    // 写入z值
    pFiler->writeDouble( m_zValue );

    // 写入坐标个数
    int len = m_pts.length();
    pFiler->writeInt32( len );

    // 写入坐标
    for( int i = 0; i < len; i++ )
    {
        pFiler->writePoint3d( m_pts[i] );
    }

    // 写入标注位置
    pFiler->writePoint3d( m_tpt );

    // 写入标注方向
    pFiler->writeDouble( m_angle );

    // 写入标注文字大小
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

    // 读取z值
    pFiler->readDouble( &m_zValue );

    // 读取坐标个数
    long len = 0;
    pFiler->readInt32( &len );

    // 读取坐标
    m_pts.removeAll();
    for( int i = 0; i < ( int )len; i++ )
    {
        AcGePoint3d pt;
        pFiler->readPoint3d( &pt );
        m_pts.append( pt );
    }

    // 读取标注位置
    pFiler->readPoint3d( &m_tpt );

    // 读取标注方向
    pFiler->readDouble( &m_angle );

    // 读取标注文字高度
    pFiler->readDouble( &m_textHeight );

    return ( pFiler->filerStatus() ) ;
}

Adesk::Boolean ContourLabel::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    AcGePoint3d pt = m_tpt;
    double angle = m_angle;

    if( m_bSmooth )
    {
        AcGePoint3dArray spts;
        ARX_SmoothPoints( m_pts, spts );

        AdjustPoint( spts, pt, angle );
    }

    // 标注文字
    DrawZValue( mode, m_zValue, pt, angle, m_textHeight );

    return Adesk::kTrue;
}

void ContourLabel::smooth( bool bSmooth )
{
    assertWriteEnabled();
    this->m_bSmooth = bSmooth;
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

void ContourLabel::setPosition( const AcGePoint3d& pt )
{
    assertWriteEnabled();
    m_tpt = pt;
}

AcGePoint3d ContourLabel::getPosition() const
{
    assertReadEnabled();
    return m_tpt;
}

void ContourLabel::update()
{
    assertWriteEnabled();
    AdjustPoint( m_pts, m_tpt, m_angle );
}