#include "StdAfx.h"
#include "SandWall.h"

Adesk::UInt32 SandWall::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    SandWall, MineGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    иЁма╠уг╫, DEFGEAPP
)

SandWall::SandWall() : MineGE()
{

}

SandWall::SandWall( const AcGePoint3d& pt, double angle, double length )
    : MineGE(), m_pt( pt ), m_angle( angle ), m_length( length )
{

}

AcGePoint3d SandWall::getInsertPt() const
{
    assertReadEnabled();
    return m_pt;
}

void SandWall::setInsertPt( const AcGePoint3d& pt )
{
    assertWriteEnabled();
    m_pt = pt;
}

double SandWall::getDirectionAngle() const
{
    assertReadEnabled();
    return m_angle;
}

void SandWall::setDirectionAngle( double angle )
{
    assertWriteEnabled();
    m_angle = angle;
}

double SandWall::getLength() const
{
    assertReadEnabled();
    return m_length;
}

void SandWall::setLength( double L )
{
    assertWriteEnabled();
    m_length = L;
}

void SandWall::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_pt );
    reader.readDouble( m_angle );
    reader.readDouble( m_length );
}

void SandWall::writeKeyParam( DrawParamWriter& writer ) const
{
    writer.writePoint( m_pt );
    writer.writeDouble( m_angle );
    writer.writeDouble( m_length );
}

Acad::ErrorStatus SandWall::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = MineGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( SandWall::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeItem( m_pt );
    pFiler->writeItem( m_angle );
    pFiler->writeItem( m_length );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus SandWall::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = MineGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > SandWall::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    pFiler->readItem( &m_pt );
    pFiler->readItem( &m_angle );
    pFiler->readItem( &m_length );

    return ( pFiler->filerStatus () ) ;
}
