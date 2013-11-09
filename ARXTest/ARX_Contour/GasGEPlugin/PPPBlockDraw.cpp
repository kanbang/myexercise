#include "StdAfx.h"
#include "PPPBlockDraw.h"
#include "DrawTool.h"

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

void PPPBlockDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    BlockDraw::readPropertyDataFromGE( values );

    m_attValues[0].append( _T( "t" ) );

    AcString str;
    str.format( _T( "m%s" ), MakeUpperText( _T( "3" ) ) );
    m_attValues[1].append( str );
}