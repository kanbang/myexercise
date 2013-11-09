#include "StdAfx.h"
#include "AFCDataLink.h"

void WS_AFCDataLink::regDatas()
{
    linkStringData( _T( "名称" ), &name );
    linkDoubleData( _T( "断面面积" ), &area );

    linkDoubleData( _T( "最大控顶距" ), &max_kdj );
    linkDoubleData( _T( "最小控顶距" ), &min_kdj );
    linkDoubleData( _T( "实际采高" ), &height );
    linkDoubleData( _T( "适宜风速" ), &reasonalVelocity );
    linkDoubleData( _T( "采高调整系数" ), &k_height );
    linkDoubleData( _T( "长度调整系数" ), &k_length );
    linkDoubleData( _T( "温度调整系数" ), &k_temperature );

    linkDoubleData( _T( "平均绝对瓦斯涌出量" ), &q_ch4 );
    linkDoubleData( _T( "瓦斯涌出不均衡通风系数" ), &k_ch4 );
    linkDoubleData( _T( "专排瓦斯巷瓦斯涌出量" ), &q_ch4_tail );

    linkIntData( _T( "工作面最多人数" ), &maxPeopleNum );
    linkDoubleData( _T( "一次爆破炸药最大用量" ), &maxWeight );
}

void TWS_AFCDataLink::regDatas()
{
    linkStringData( _T( "名称" ), &name );
    linkDoubleData( _T( "断面面积" ), &area );
    linkIntData( _T( "材质类型" ), &tunnelMaterial );

    linkDoubleData( _T( "平均绝对瓦斯涌出量" ), &q_ch4 );
    linkDoubleData( _T( "瓦斯涌出不均衡通风系数" ), &k_ch4 );

    linkIntData( _T( "局部通风机台数" ), &fanNum );
    linkDoubleData( _T( "局部通风机实际吸风量" ), &q_fan );

    linkIntData( _T( "工作面最多人数" ), &maxPeopleNum );
    linkDoubleData( _T( "一次爆破炸药最大用量" ), &maxWeight );
}

void Powder_AFCDataLink::regDatas()
{
    linkStringData( _T( "名称" ), &name );
    linkDoubleData( _T( "体积" ), &V );
}

void Charge_AFCDataLink::regDatas()
{
    linkStringData( _T( "名称" ), &name );
    linkDoubleData( _T( "氢气量" ), &q_h2 );
}

void Machine_AFCDataLink::regDatas()
{
    linkStringData( _T( "名称" ), &name );
    linkDoubleData( _T( "功率" ), &power );
    linkDoubleData( _T( "发热系数" ), &theta );
    linkDoubleData( _T( "温度差" ), &delta_t );
}

Tunnel_AFCDataLink::Tunnel_AFCDataLink()
{

}

Tunnel_AFCDataLink::~Tunnel_AFCDataLink()
{
}

void Tunnel_AFCDataLink::regDatas()
{
    linkBoolData( _T( "是否单独配风" ), &needAir );

    linkStringData( _T( "名称" ), &name );
    linkDoubleData( _T( "断面面积" ), &area );
    linkIntData( _T( "材质类型" ), &tunnelMaterial );

    linkDoubleData( _T( "平均绝对瓦斯涌出量" ), &q_ch4 );

    linkBoolData( _T( "运行架线电机车" ), &hasElectricWireCar );

    linkIntData( _T( "胶轮车台数" ), &num );
    linkDoubleData( _T( "胶轮车功率" ), &power );
}