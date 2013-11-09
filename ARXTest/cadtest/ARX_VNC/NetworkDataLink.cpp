#include "StdAfx.h"
#include "NetworkDataLink.h"

EdgeDataLink::EdgeDataLink() : r( 0 ), q( 0 ), ar( 0 ), fq( 0 )
{

}

void EdgeDataLink::regDatas()
{
    linkStringData( _T( "����" ), &name );
    linkDoubleData( _T( "����" ), &r );
    linkDoubleData( _T( "���ڷ���" ), &ar );
    linkDoubleData( _T( "����" ), &q );
    linkDoubleData( _T( "�̶�����" ), &fq );
}

FanDataLink::FanDataLink() : a0( 0 ), a1( 0 ), a2( 0 ), a3( 0 ), a4( 0 ), a5( 0 )
{

}

void FanDataLink::regDatas()
{
    linkStringData( _T( "����" ), &name );
    linkDoubleData( _T( "����-��ѹ��������0����" ), &a0 );
    linkDoubleData( _T( "����-��ѹ��������1����" ), &a1 );
    linkDoubleData( _T( "����-��ѹ��������2����" ), &a2 );
    linkDoubleData( _T( "����-��ѹ��������3����" ), &a3 );
    linkDoubleData( _T( "����-��ѹ��������4����" ), &a4 );
    linkDoubleData( _T( "����-��ѹ��������5����" ), &a5 );
}

GateDataLink::GateDataLink() : r( 0 )
{

}

void GateDataLink::regDatas()
{
    linkDoubleData( _T( "��Ч����" ), &r );
}

void ChimneyDataLink::regDatas()
{
    linkDoubleData( _T( "����" ), &r );
}
