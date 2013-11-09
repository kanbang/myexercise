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
//----- DocReactor.cpp : Implementation of DocReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "DocReactor.h"

#include "Utils.h"

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS( DocReactor, AcApDocManagerReactor, 1 )

//-----------------------------------------------------------------------------
DocReactor::DocReactor ( const bool autoInitAndRelease ) : AcApDocManagerReactor(), mbAutoInitAndRelease( autoInitAndRelease )
{
    if ( autoInitAndRelease )
    {
        if ( acDocManager )
            acDocManager->addReactor ( this ) ;
        else
            mbAutoInitAndRelease = false ;
    }
}

//-----------------------------------------------------------------------------
DocReactor::~DocReactor ()
{
    Detach () ;
}

//-----------------------------------------------------------------------------
void DocReactor::Attach ()
{
    Detach () ;
    if ( !mbAutoInitAndRelease )
    {
        if ( acDocManager )
        {
            acDocManager->addReactor ( this ) ;
            mbAutoInitAndRelease = true ;
        }
    }
}

void DocReactor::Detach ()
{
    if ( mbAutoInitAndRelease )
    {
        if ( acDocManager )
        {
            acDocManager->removeReactor ( this ) ;
            mbAutoInitAndRelease = false ;
        }
    }
}

AcApDocManager* DocReactor::Subject () const
{
    return ( acDocManager ) ;
}

bool DocReactor::IsAttached () const
{
    return ( mbAutoInitAndRelease ) ;
}


// -----------------------------------------------------------------------------
void DocReactor::documentToBeDestroyed( AcApDocument* pDocToDestroy )
{
    AcApDocManagerReactor::documentToBeDestroyed ( pDocToDestroy ) ;

    AfxMessageBox( _T( "�ĵ�����������" ) );
}

// -----------------------------------------------------------------------------
void DocReactor::documentDestroyed( const ACHAR* fileName )
{
    //CString msg;
    //msg.Format(_T("����ǰ�ĵ�����:%d"), acDocManager->documentCount());
    //AfxMessageBox(msg);
    if( acDocManager->documentCount() == 1 )
    {
        AfxMessageBox( _T( "����һ���ĵ�" ) );
        AfxMessageBox( _T( "�ĵ����ٿ�ʼ" ) );
        //UnLoadPartialMenuUseCom(CUI_FILE_GROUP_NAME);
        UnLoadPartialMenuUseCmd( CUI_FILE_GROUP_NAME );
        AfxMessageBox( _T( "�ĵ����ٽ���" ) );
    }

    AcApDocManagerReactor::documentDestroyed ( fileName ) ;

    //msg.Format(_T("���ٺ��ĵ�����:%d"), acDocManager->documentCount());
    //AfxMessageBox(msg);

    //msg.Format(_T("�ĵ�:\n%s\n������"), ss);
    //AfxMessageBox(msg);
}
