#include "StdAfx.h"
#include "VNG_GE.h"

Adesk::UInt32 VNG_GE::kCurrentVersionNumber = 1 ;

ACRX_NO_CONS_DEFINE_MEMBERS( VNG_GE, AcDbEntity )

VNG_GE::VNG_GE () : AcDbEntity ()
{
}

VNG_GE::~VNG_GE ()
{
}

Acad::ErrorStatus VNG_GE::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = AcDbEntity::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( VNG_GE::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeInt32( m_id );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus VNG_GE::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = AcDbEntity::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > VNG_GE::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    long L;
    pFiler->readInt32( &L );
    m_id = ( int )L;

    return ( pFiler->filerStatus () ) ;
}