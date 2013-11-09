#include "StdAfx.h"
#include "AFCDataLink.h"

void AFCDataLink::regDatas()
{
    linkStringData( _T( "����" ), &name );
    linkDoubleData( _T( "����" ), &q );
    linkDoubleData( _T( "������" ), &userQ );
}

void WS_AFCDataLink::regDatas()
{
    AFCDataLink::regDatas();

    linkDoubleData( _T( "�������" ), &area );
    linkDoubleData( _T( "���ض���" ), &max_kdj );
    linkDoubleData( _T( "��С�ض���" ), &min_kdj );
    linkDoubleData( _T( "ʵ�ʲɸ�" ), &height );
    linkDoubleData( _T( "���˷���" ), &reasonalVelocity );
    linkDoubleData( _T( "�ɸߵ���ϵ��" ), &k_height );
    linkDoubleData( _T( "���ȵ���ϵ��" ), &k_length );
    linkDoubleData( _T( "�¶ȵ���ϵ��" ), &k_temperature );

    linkDoubleData( _T( "ƽ��������˹ӿ����" ), &q_ch4 );
    linkDoubleData( _T( "��˹ӿ��������ͨ��ϵ��" ), &k_ch4 );
    linkDoubleData( _T( "ר����˹����˹ӿ����" ), &q_ch4_tail );

    linkIntData( _T( "�������������" ), &maxPeopleNum );
    linkDoubleData( _T( "һ�α���ըҩ�������" ), &maxWeight );
}

void TWS_AFCDataLink::regDatas()
{
    AFCDataLink::regDatas();

    linkDoubleData( _T( "�������" ), &area );
    linkIntData( _T( "��������" ), &tm );

    linkDoubleData( _T( "ƽ��������˹ӿ����" ), &q_ch4 );
    linkDoubleData( _T( "��˹ӿ��������ͨ��ϵ��" ), &k_ch4 );

    linkIntData( _T( "�ֲ�ͨ���̨��" ), &fanNum );
    linkDoubleData( _T( "�ֲ�ͨ���ʵ��������" ), &q_fan );

    linkIntData( _T( "�������������" ), &maxPeopleNum );
    linkDoubleData( _T( "һ�α���ըҩ�������" ), &maxWeight );
}

void Powder_AFCDataLink::regDatas()
{
    AFCDataLink::regDatas();
    linkDoubleData( _T( "���" ), &V );
}

void Charge_AFCDataLink::regDatas()
{
    AFCDataLink::regDatas();
    linkDoubleData( _T( "������" ), &q_h2 );
}

void Machine_AFCDataLink::regDatas()
{
    AFCDataLink::regDatas();
    linkDoubleData( _T( "����" ), &power );
    linkDoubleData( _T( "����ϵ��" ), &theta );
    linkDoubleData( _T( "�¶Ȳ�" ), &delta_t );
}

Tunnel_AFCDataLink::Tunnel_AFCDataLink()
{

}

Tunnel_AFCDataLink::~Tunnel_AFCDataLink()
{

}

void Tunnel_AFCDataLink::regDatas()
{
    AFCDataLink::regDatas();
    linkBoolData( _T( "�Ƿ񵥶����" ), &needAir );
    linkDoubleData( _T( "�������" ), &area );
    linkIntData( _T( "��������" ), &tm );

    linkDoubleData( _T( "ƽ��������˹ӿ����" ), &q_ch4 );

    linkIntData( _T( "��������" ), &ft );
    linkIntData( _T( "���ֳ�̨��" ), &num );
    linkDoubleData( _T( "���ֳ�����" ), &power );
}