#include "StdAfx.h"
#include "ArcTunnel.h"

Adesk::UInt32 ArcTunnel::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    ArcTunnel, Tunnel,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    »¡ÏßÏïµÀ, DEFGEAPP
)

ArcTunnel::ArcTunnel () : Tunnel ()
{
}

ArcTunnel::ArcTunnel( const AcGePoint3d& startPt, const AcGePoint3d& endPt, const AcGePoint3d& thirdPt )
    : Tunnel( startPt, endPt ), m_thirdPt( thirdPt )
{

}

ArcTunnel::~ArcTunnel ()
{
}

AcGePoint3d ArcTunnel::getThirdPt() const
{
    assertReadEnabled();

    return m_thirdPt;
}

void ArcTunnel::setThirdPt( const AcGePoint3d& pt )
{
    assertWriteEnabled();

    m_thirdPt = pt;
}

Acad::ErrorStatus ArcTunnel::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;

    Acad::ErrorStatus es = Tunnel::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;

    if ( ( es = pFiler->writeUInt32 ( ArcTunnel::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeItem( m_thirdPt );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus ArcTunnel::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;

    Acad::ErrorStatus es = Tunnel::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;

    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > ArcTunnel::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    pFiler->readItem( &m_thirdPt );

    return ( pFiler->filerStatus () ) ;
}

void ArcTunnel::pushKeyParamToWriter( DrawParamWriter& writer ) const
{
    assertReadEnabled();

    LinkedGE::pushKeyParamToWriter( writer );
    writer.writePoint( m_thirdPt );
}

void ArcTunnel::pullKeyParamFromReader( DrawParamReader& reader )
{
    assertWriteEnabled();

    LinkedGE::pullKeyParamFromReader( reader );
    reader.readPoint( m_thirdPt );
}