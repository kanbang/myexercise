#include "StdAfx.h"
#include "GFPBlockDraw.h"

ACRX_CONS_DEFINE_MEMBERS( WGFPBlockDraw, BlockDraw, 1 )
ACRX_CONS_DEFINE_MEMBERS( TGFPBlockDraw, BlockDraw, 1 )

WGFPBlockDraw::WGFPBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "�زɹ�������˹ӿ������" );
}

void WGFPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "������˹ӿ����" ) );
    names.append( _T( "�����˹ӿ����" ) );
    names.append( _T( "�������ղ���" ) );
    names.append( _T( "�ز�����" ) );
}

TGFPBlockDraw::TGFPBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "��������������˹ӿ������" );
}

void TGFPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "������˹ӿ����" ) );
    names.append( _T( "�������" ) );
}