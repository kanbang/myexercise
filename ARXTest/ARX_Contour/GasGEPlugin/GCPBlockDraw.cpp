#include "StdAfx.h"
#include "GCPBlockDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS( GCPBlockDraw, BlockDraw, 1 )

GCPBlockDraw::GCPBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "��˹������" );
}

void GCPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "��˹����" ) );
    names.append( _T( "�����" ) );
    names.append( _T( "����" ) );
}

void GCPBlockDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    BlockDraw::readPropertyDataFromGE( values );

    AcString str;
    str.format( _T( "m%s/t" ), MakeUpperText( _T( "3" ) ) );
    m_attValues[0].append( str );
}