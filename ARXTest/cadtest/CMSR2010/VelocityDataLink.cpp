#include "StdAfx.h"
#include "VelocityDataLink.h"

VelocityDataLink::VelocityDataLink()
{
}

void VelocityDataLink::regDatas()
{
    linkStringData( _T( "����" ), &name );
    linkDoubleData( _T( "����" ), &v );
    linkDoubleData( _T( "ƽ��������˹ӿ����" ), &q_ch4 );
}

TunnelVelocityDataLink::TunnelVelocityDataLink()
{
    type = VD_TUNNEL;
}

void TunnelVelocityDataLink::regDatas()
{
    VelocityDataLink::regDatas();
    linkIntData( _T( "��������" ), &ft );
    linkIntData( _T( "��Ͳ��������" ), &sp );
}

WSVelocityDataLink::WSVelocityDataLink()
{
    type = VD_WS;
}

void WSVelocityDataLink::regDatas()
{
    VelocityDataLink::regDatas();
    linkIntData( _T( "��������" ), &tm );
    linkIntData( _T( "��ú����" ), &cm );
}

TWSVelocityDataLink::TWSVelocityDataLink()
{
    type = VD_TWS;
}

void TWSVelocityDataLink::regDatas()
{
    VelocityDataLink::regDatas();
    linkIntData( _T( "��������" ), &tm );
}

BridgeVelocityDataLink::BridgeVelocityDataLink()
{
    type = VD_BRIDGE;
}