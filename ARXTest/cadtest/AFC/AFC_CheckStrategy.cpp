#include "StdAfx.h"
#include "AFC_CheckStrategy.h"

namespace PJ
{
    AFC_CheckStrategy::AFC_CheckStrategy( const CString& tableName ) : AFCStrategy( tableName )
    {
    }

    double AFC_CheckStrategy::caculate()
    {
        double Q_last = getLastQ();
        double Q_min = minQ();
        double Q_max = maxQ();
        if( Q_last < Q_min ) return Q_min;
        if( Q_last > Q_max ) return -1; // 错误（超出最大风量）
        return Q_last;
    }

    double AFC_CheckStrategy::minQ()
    {
        return 0;
    }

    double AFC_CheckStrategy::maxQ()
    {
        return 10000;
    }

    void AFC_CheckStrategy::createTableImpl( TableCreator* pTableCreator )
    {
        BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
        DEFINE_REAL_FIELD( Q_min )
        DEFINE_REAL_FIELD( Q_max )
        END_DEFINE_FIELD
    }

    void AFC_CheckStrategy::writeToTableImpl( DataWriter* pDataWriter )
    {
        BEGIN_WRITE_DATA( DataWriter, pDataWriter )
        WRITE_REAL_DATA( Q_min, minQ() )
        WRITE_REAL_DATA( Q_max, maxQ() )
        END_WRITE_DATA
    }
}