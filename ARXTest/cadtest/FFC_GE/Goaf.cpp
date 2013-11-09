#include "StdAfx.h"
#include "Goaf.h"

Adesk::UInt32 Goaf::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    Goaf, MineGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    采空区, DEFGEAPP
)

Goaf::Goaf () : MineGE ()
{
}

Goaf::~Goaf ()
{
}

void Goaf::addPoint( const AcGePoint3d& pt )
{
    assertWriteEnabled();
    if( !m_pts.contains( pt ) )
    {
        m_pts.append( pt );
    }
}

void Goaf::getPolygon( AcGePoint3dArray& polygon ) const
{
    assertReadEnabled();
    polygon.append( m_pts );
}

static AcGePoint3d CacLineClosePt( const AcGePoint3d& spt, const AcGePoint3d& ept, const AcGePoint3d& pt )
{
    // 构造一条几何线段
    AcGeLineSeg3d line( spt, ept );
    return line.closestPointTo( pt ); // 计算距离中线最近的点
}

void Goaf::splitLine( int pos, const AcGePoint3d& pt )
{
    assertWriteEnabled();

    int n = m_pts.length();
    if( pos < 0 || pos >= n ) return;
    if( m_pts.contains( pt ) ) return;

    m_pts.insertAt( pos + 1, CacLineClosePt( m_pts[pos], m_pts[( pos + 1 ) % n], pt ) );
}

void Goaf::removePoint( int pos )
{
    assertWriteEnabled();

    int n = m_pts.length();
    if( n < 5 ) return; // 至少是一个四边形
    if( pos < 0 || pos >= n ) return;

    m_pts.removeAt( pos );
}

void Goaf::setPoint( int pos, const AcGePoint3d& pt )
{
    assertWriteEnabled();

    int n = m_pts.length();
    if( pos < 0 || pos >= n ) return;
    if( m_pts.contains( pt ) ) return;

    m_pts[pos] = pt;
}

void Goaf::writeKeyParam( DrawParamWriter& writer ) const
{
    writer.writeInt( m_pts.length() );
    for( int i = 0; i < m_pts.length(); i++ )
    {
        writer.writePoint( m_pts[i] );
    }
}

void Goaf::readKeyParam( DrawParamReader& reader )
{
    int n = 0;
    reader.readInt( n );

    m_pts.removeAll();
    for( int i = 0; i < n; i++ )
    {
        AcGePoint3d pt;
        reader.readPoint( pt );
        m_pts.append( pt );
    }
}

Acad::ErrorStatus Goaf::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;

    Acad::ErrorStatus es = MineGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;

    if ( ( es = pFiler->writeUInt32( Goaf::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeInt32( m_pts.length() );
    for( int i = 0; i < m_pts.length(); i++ )
    {
        pFiler->writePoint3d( m_pts[i] );
    }

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus Goaf::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;

    Acad::ErrorStatus es = MineGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;

    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > Goaf::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    long n = 0;
    pFiler->readInt32( &n );

    m_pts.removeAll();
    for( int i = 0; i < ( int )n; i++ )
    {
        AcGePoint3d pt;
        pFiler->readPoint3d( &pt );
        m_pts.append( pt );
    }

    return ( pFiler->filerStatus () ) ;
}
