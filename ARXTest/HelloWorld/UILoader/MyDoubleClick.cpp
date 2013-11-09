#include "StdAfx.h"
#include "MyDoubleClick.h"

//-----------------------------------------------------------------------------
MyDoubleClick::MyDoubleClick ( const bool autoInitAndRelease ) : AcEditorReactor(), mbAutoInitAndRelease( autoInitAndRelease )
{
    if ( autoInitAndRelease )
    {
        if ( acedEditor )
            acedEditor->addReactor ( this ) ;
        else
            mbAutoInitAndRelease = false ;
    }
}

//-----------------------------------------------------------------------------
MyDoubleClick::~MyDoubleClick ()
{
    Detach () ;
}

//-----------------------------------------------------------------------------
void MyDoubleClick::Attach ()
{
    Detach () ;
    if ( !mbAutoInitAndRelease )
    {
        if ( acedEditor )
        {
            acedEditor->addReactor ( this ) ;
            mbAutoInitAndRelease = true ;
        }
    }
}

void MyDoubleClick::Detach ()
{
    if ( mbAutoInitAndRelease )
    {
        if ( acedEditor )
        {
            acedEditor->removeReactor ( this ) ;
            mbAutoInitAndRelease = false ;
        }
    }
}

AcEditor* MyDoubleClick::Subject () const
{
    return ( acedEditor ) ;
}

bool MyDoubleClick::IsAttached () const
{
    return ( mbAutoInitAndRelease ) ;
}

void MyDoubleClick::beginDoubleClick( const AcGePoint3d& clickPoint )
{
    AcEditorReactor::beginDoubleClick ( clickPoint );
    AfxMessageBox( _T( "µ¯³ö¶Ô»°¿ò1" ) );
}
