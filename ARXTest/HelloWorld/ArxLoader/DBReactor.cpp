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
//----- DBReactor.cpp : Implementation of DBReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "DBReactor.h"

#include "Utils.h"

//-----------------------------------------------------------------------------
DBReactor::DBReactor ( const bool autoInitAndRelease ) : AcRxEventReactor(), mbAutoInitAndRelease( autoInitAndRelease )
{
    if ( autoInitAndRelease )
    {
        if ( acrxEvent )
            acrxEvent->addReactor ( this ) ;
        else
            mbAutoInitAndRelease = false ;
    }
}

//-----------------------------------------------------------------------------
DBReactor::~DBReactor ()
{
    Detach () ;
}

//-----------------------------------------------------------------------------
void DBReactor::Attach ()
{
    Detach () ;
    if ( !mbAutoInitAndRelease )
    {
        if ( acrxEvent )
        {
            acrxEvent->addReactor ( this ) ;
            mbAutoInitAndRelease = true ;
        }
    }
}

void DBReactor::Detach ()
{
    if ( mbAutoInitAndRelease )
    {
        if ( acrxEvent )
        {
            acrxEvent->removeReactor ( this ) ;
            mbAutoInitAndRelease = false ;
        }
    }
}

AcRxEvent* DBReactor::Subject () const
{
    return ( acrxEvent ) ;
}

bool DBReactor::IsAttached () const
{
    return ( mbAutoInitAndRelease ) ;
}

// -----------------------------------------------------------------------------
void DBReactor::databaseToBeDestroyed( AcDbDatabase* param2 )
{
    AcRxEventReactor::databaseToBeDestroyed ( param2 ) ;

    AfxMessageBox( _T( "数据库销毁开始" ) );
    //UnLoadPartialMenuUseCom(CUI_FILE_GROUP_NAME);
    UnLoadPartialMenuUseCmd( CUI_FILE_GROUP_NAME );
    AfxMessageBox( _T( "数据库销毁结束" ) );
}
