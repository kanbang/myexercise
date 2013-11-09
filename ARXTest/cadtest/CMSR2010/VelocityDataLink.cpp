#include "StdAfx.h"
#include "VelocityDataLink.h"

VelocityDataLink::VelocityDataLink()
{
}

void VelocityDataLink::regDatas()
{
    linkStringData( _T( "名称" ), &name );
    linkDoubleData( _T( "风速" ), &v );
    linkDoubleData( _T( "平均绝对瓦斯涌出量" ), &q_ch4 );
}

TunnelVelocityDataLink::TunnelVelocityDataLink()
{
    type = VD_TUNNEL;
}

void TunnelVelocityDataLink::regDatas()
{
    VelocityDataLink::regDatas();
    linkIntData( _T( "功能类型" ), &ft );
    linkIntData( _T( "井筒附加属性" ), &sp );
}

WSVelocityDataLink::WSVelocityDataLink()
{
    type = VD_WS;
}

void WSVelocityDataLink::regDatas()
{
    VelocityDataLink::regDatas();
    linkIntData( _T( "材质类型" ), &tm );
    linkIntData( _T( "采煤方法" ), &cm );
}

TWSVelocityDataLink::TWSVelocityDataLink()
{
    type = VD_TWS;
}

void TWSVelocityDataLink::regDatas()
{
    VelocityDataLink::regDatas();
    linkIntData( _T( "材质类型" ), &tm );
}

BridgeVelocityDataLink::BridgeVelocityDataLink()
{
    type = VD_BRIDGE;
}