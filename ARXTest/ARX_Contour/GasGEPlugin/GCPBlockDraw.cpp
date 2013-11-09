#include "StdAfx.h"
#include "GCPBlockDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS( GCPBlockDraw, BlockDraw, 1 )

GCPBlockDraw::GCPBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "瓦斯含量点" );
}

void GCPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "瓦斯含量" ) );
    names.append( _T( "测点标高" ) );
    names.append( _T( "埋深" ) );
}

void GCPBlockDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    BlockDraw::readPropertyDataFromGE( values );

    AcString str;
    str.format( _T( "m%s/t" ), MakeUpperText( _T( "3" ) ) );
    m_attValues[0].append( str );
}