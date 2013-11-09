#include "StdAfx.h"
#include "GPPBlockDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS( GPPBlockDraw, BlockDraw, 1 )

GPPBlockDraw::GPPBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "瓦斯压力点" );
}

void GPPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "瓦斯压力值" ) );
    names.append( _T( "测点标高" ) );
    names.append( _T( "埋深" ) );
}

void GPPBlockDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    BlockDraw::readPropertyDataFromGE( values );

    m_attValues[0].append( _T( "MPa" ) );
}