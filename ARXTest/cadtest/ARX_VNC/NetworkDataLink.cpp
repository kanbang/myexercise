#include "StdAfx.h"
#include "NetworkDataLink.h"

EdgeDataLink::EdgeDataLink() : r( 0 ), q( 0 ), ar( 0 ), fq( 0 )
{

}

void EdgeDataLink::regDatas()
{
    linkStringData( _T( "名称" ), &name );
    linkDoubleData( _T( "风阻" ), &r );
    linkDoubleData( _T( "调节风阻" ), &ar );
    linkDoubleData( _T( "风量" ), &q );
    linkDoubleData( _T( "固定风量" ), &fq );
}

FanDataLink::FanDataLink() : a0( 0 ), a1( 0 ), a2( 0 ), a3( 0 ), a4( 0 ), a5( 0 )
{

}

void FanDataLink::regDatas()
{
    linkStringData( _T( "名称" ), &name );
    linkDoubleData( _T( "风量-风压特性曲线0次项" ), &a0 );
    linkDoubleData( _T( "风量-风压特性曲线1次项" ), &a1 );
    linkDoubleData( _T( "风量-风压特性曲线2次项" ), &a2 );
    linkDoubleData( _T( "风量-风压特性曲线3次项" ), &a3 );
    linkDoubleData( _T( "风量-风压特性曲线4次项" ), &a4 );
    linkDoubleData( _T( "风量-风压特性曲线5次项" ), &a5 );
}

GateDataLink::GateDataLink() : r( 0 )
{

}

void GateDataLink::regDatas()
{
    linkDoubleData( _T( "等效风阻" ), &r );
}

void ChimneyDataLink::regDatas()
{
    linkDoubleData( _T( "风阻" ), &r );
}
