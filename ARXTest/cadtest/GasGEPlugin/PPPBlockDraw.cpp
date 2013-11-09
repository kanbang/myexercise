#include "StdAfx.h"
#include "PPPBlockDraw.h"

ACRX_CONS_DEFINE_MEMBERS( PPPBlockDraw, BlockDraw, 1 )

PPPBlockDraw::PPPBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "���������" );
}

void PPPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "ͻ��ú����" ) );
    names.append( _T( "ӿ����˹��" ) );
    names.append( _T( "���" ) );
    names.append( _T( "��������" ) );
}