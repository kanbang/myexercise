#include "StdAfx.h"
#include "GCPBlockDraw.h"

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