#include "StdAfx.h"
#include "SelectedGE_EditorReactor.h"

#include "UIHelper.h"
#include "../ArxHelper/HelperClass.h"

SelectedGE_EditorReactor::SelectedGE_EditorReactor ( const bool autoInitAndRelease )
    : AcEditorReactor(), mbAutoInitAndRelease( autoInitAndRelease ), m_bMonitorPickFirst( false )
{
    if( autoInitAndRelease )
    {
        if ( acedEditor )
            acedEditor->addReactor ( this ) ;
        else
            mbAutoInitAndRelease = false ;
    }
}

SelectedGE_EditorReactor::~SelectedGE_EditorReactor ()
{
    Detach () ;
}

void SelectedGE_EditorReactor::Attach ()
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

void SelectedGE_EditorReactor::Detach ()
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

AcEditor* SelectedGE_EditorReactor::Subject () const
{
    return ( acedEditor ) ;
}

bool SelectedGE_EditorReactor::IsAttached () const
{
    return ( mbAutoInitAndRelease ) ;
}

void SelectedGE_EditorReactor::pickfirstModified( void )
{
    if( !m_bMonitorPickFirst ) return;

    AcDbObjectIdArray objIds;
    ArxUtilHelper::GetPickSetEntity( objIds );

    CAcModuleResourceOverride resourceOverride;

    if( objIds.isEmpty() )
    {
        UIHelper::LoadData( AcDbObjectId::kNull );
    }
    else if( objIds.length() == 1 )
    {
        UIHelper::LoadData( objIds[0] );
    }
    else
    {
        UIHelper::LoadData( AcDbObjectId::kNull );
    }

    //AcEditorReactor::pickfirstModified();
}
