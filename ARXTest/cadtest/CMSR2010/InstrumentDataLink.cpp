#include "StdAfx.h"
#include "InstrumentDataLink.h"

void InstrumentDataLink::regDatas()
{
    linkStringData( _T( "����" ), &name );
    linkStringData( _T( "�ͺ�" ), &mn );
    linkIntData( _T( "����" ), &amount );
    linkBoolData( _T( "����������" ), &qi );
}