#include "StdAfx.h"
#include "NullAFCStrategy.h"

namespace PJ
{
    NullAFCStrategy::NullAFCStrategy( const CString& tableName ) : AFCStrategy( tableName )
    {
    }

    void NullAFCStrategy::doCreateTable( TableCreator* pTableCreator )
    {

    }

    void NullAFCStrategy::doWriteToTable( DataWriter* pDataWriter )
    {
    }

    double NullAFCStrategy::caculate()
    {
        return 0;
    }
}