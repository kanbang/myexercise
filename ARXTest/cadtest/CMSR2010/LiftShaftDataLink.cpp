#include "StdAfx.h"
#include "LiftShaftDataLink.h"

void LiftShaftDataLink::regDatas()
{
    linkStringData( _T( "����" ), &name );
    linkDoubleData( _T( "����" ), &v );
    linkIntData( _T( "��������" ), &ft );
    linkIntData( _T( "��Ͳ��������" ), &sp );
}