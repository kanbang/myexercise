#include "StdAfx.h"
#include "TempDataLink.h"

void TempDataLink::regDatas()
{
    linkStringData( _T( "����" ), &name );
    linkDoubleData( _T( "�¶�" ), &t );
    linkIntData( _T( "���¿��ƴ�ʩ" ), &cm );
}