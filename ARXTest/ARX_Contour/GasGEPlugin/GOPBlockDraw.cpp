#include "StdAfx.h"
#include "GOPBlockDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS( GOPBlockDraw, BlockDraw, 1 )

GOPBlockDraw::GOPBlockDraw () : BlockDraw ()
{
}

void GOPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "突出强度" ) );
    names.append( _T( "突出煤量" ) );
    names.append( _T( "涌出瓦斯总量" ) );
    names.append( _T( "标高" ) );
    names.append( _T( "突出年月日" ) );
}

void GOPBlockDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    BlockDraw::readPropertyDataFromGE( values );

    // 增加单位
    m_attValues[1].append( _T( "t" ) ); // 单位:t

    AcString str;
    str.format( _T( "万m%s" ), MakeUpperText( _T( "3" ) ) );
    m_attValues[2].append( str );
    m_blockName = m_attValues[0].kACharPtr();
}

