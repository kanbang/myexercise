#include "StdAfx.h"
#include "GasGeologyGE.h"

Adesk::UInt32 GasGeologyGE::kCurrentVersionNumber = 1 ;

ACRX_NO_CONS_DEFINE_MEMBERS( GasGeologyGE, GasGE )

GasGeologyGE::GasGeologyGE() : m_angle( 0 )
{

}

GasGeologyGE::GasGeologyGE( const AcGePoint3d& insertPt ) : m_insertPt( insertPt ), m_angle( 0 )
{

}

AcGePoint3d GasGeologyGE::getInsertPt() const
{
    assertReadEnabled();
    return m_insertPt;
}

void GasGeologyGE::setInsertPt( const AcGePoint3d& pt )
{
    assertWriteEnabled();
    m_insertPt = pt;
}

void GasGeologyGE::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void GasGeologyGE::writeKeyParam( DrawParamWriter& writer ) const
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

Acad::ErrorStatus GasGeologyGE::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = GasGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( GasGeologyGE::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeItem( m_insertPt );
    pFiler->writeItem( m_angle );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus GasGeologyGE::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = GasGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > GasGeologyGE::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    pFiler->readItem( &m_insertPt );
    pFiler->readItem( &m_angle );

    return ( pFiler->filerStatus () ) ;
}