#include "StdAfx.h"
#include "GPPBlockDraw.h"

ACRX_CONS_DEFINE_MEMBERS( GPPBlockDraw, BlockDraw, 1 )

GPPBlockDraw::GPPBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "��˹ѹ����" );
}

void GPPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "��˹ѹ��ֵ" ) );
    names.append( _T( "�����" ) );
    names.append( _T( "����" ) );
}