#include "StdAfx.h"
#include "CDHBlockDraw.h"

ACRX_CONS_DEFINE_MEMBERS( CDHBlockDraw, BlockDraw, 1 )

CDHBlockDraw::CDHBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "��ú���" );
}

void CDHBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "�׺�" ) );
    names.append( _T( "������" ) );
    names.append( _T( "ú��װ���" ) );
    names.append( _T( "ú��" ) );
}