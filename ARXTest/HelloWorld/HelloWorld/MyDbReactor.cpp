// (C) Copyright 2002-2007 by Autodesk, Inc.
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted,
// provided that the above copyright notice appears in all copies and
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS.
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC.
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- MyDbReactor.cpp : Implementation of MyDbReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "MyDbReactor.h"

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS( MyDbReactor, AcDbDatabaseReactor, 1 )

//-----------------------------------------------------------------------------
MyDbReactor::MyDbReactor ( AcDbDatabase* pDb ) : AcDbDatabaseReactor(), mpDatabase( pDb )
{
    if ( pDb )
        pDb->addReactor ( this ) ;
}

//-----------------------------------------------------------------------------
MyDbReactor::~MyDbReactor ()
{
    Detach () ;
}

//-----------------------------------------------------------------------------
void MyDbReactor::Attach ( AcDbDatabase* pDb )
{
    Detach () ;
    if ( mpDatabase == NULL )
    {
        if ( ( mpDatabase = pDb ) != NULL )
            pDb->addReactor ( this ) ;
    }
}

void MyDbReactor::Detach ()
{
    if ( mpDatabase )
    {
        mpDatabase->removeReactor ( this ) ;
        mpDatabase = NULL ;
    }
}

AcDbDatabase* MyDbReactor::Subject () const
{
    return ( mpDatabase ) ;
}

bool MyDbReactor::IsAttached () const
{
    return ( mpDatabase != NULL ) ;
}

static void PrintMsg( const AcDbObject* dbObj, const CString& funcName )
{
    acutPrintf( _T( "id:%d type:%s %s\n" ), dbObj->objectId().asOldId(), dbObj->isA()->name(), funcName );

}

// -----------------------------------------------------------------------------
void MyDbReactor::objectAppended( const AcDbDatabase* dwg, const AcDbObject* dbObj )
{
    AcDbDatabaseReactor::objectAppended ( dwg, dbObj ) ;
    PrintMsg( dbObj, _T( "objectAppended" ) );
}

// -----------------------------------------------------------------------------
void MyDbReactor::objectUnAppended( const AcDbDatabase* dwg, const AcDbObject* dbObj )
{
    AcDbDatabaseReactor::objectUnAppended ( dwg, dbObj ) ;
    PrintMsg( dbObj, _T( "objectUnAppended" ) );
}

// -----------------------------------------------------------------------------
void MyDbReactor::objectReAppended( const AcDbDatabase* dwg, const AcDbObject* dbObj )
{
    AcDbDatabaseReactor::objectReAppended ( dwg, dbObj ) ;
    PrintMsg( dbObj, _T( "objectReAppended" ) );
}

// -----------------------------------------------------------------------------
void MyDbReactor::objectOpenedForModify( const AcDbDatabase* dwg, const AcDbObject* dbObj )
{
    AcDbDatabaseReactor::objectOpenedForModify ( dwg, dbObj ) ;
    PrintMsg( dbObj, _T( "objectOpenedForModify" ) );
}

// -----------------------------------------------------------------------------
void MyDbReactor::objectModified( const AcDbDatabase* dwg, const AcDbObject* dbObj )
{
    AcDbDatabaseReactor::objectModified ( dwg, dbObj ) ;
    PrintMsg( dbObj, _T( "objectModified" ) );
}

// -----------------------------------------------------------------------------
void MyDbReactor::objectErased( const AcDbDatabase* dwg, const AcDbObject* dbObj, Adesk::Boolean pErased )
{
    AcDbDatabaseReactor::objectErased ( dwg, dbObj, pErased ) ;
    PrintMsg( dbObj, _T( "objectErased" ) );
}
