#include "StdAfx.h"
#include "InstrumentDataLink.h"

void InstrumentDataLink::regDatas()
{
    linkStringData( _T( "名称" ), &name );
    linkStringData( _T( "型号" ), &mn );
    linkIntData( _T( "数量" ), &amount );
    linkBoolData( _T( "计量检验结果" ), &qi );
}