#include "StdAfx.h"
#include "Gas_AFCStrategy.h"

namespace PJ
{
    Gas_AFCStrategy::Gas_AFCStrategy( const CString& tableName ) : AFCStrategy( tableName )
    {
        q_ch4_tail = 0;
    }

    void Gas_AFCStrategy::setQ_CH4( double q )
    {
        this->q_ch4 = q;
    }

    void Gas_AFCStrategy::setK_CH4( double k )
    {
        this->k_ch4 = k;
    }

    void Gas_AFCStrategy::setQ_CH4_TAIL( double q )
    {
        this->q_ch4_tail = q;
    }

    double Gas_AFCStrategy::caculate()
    {
        return 100 * q_ch4 * k_ch4 + q_ch4_tail * k_ch4 * 0.025;
    }

    void Gas_AFCStrategy::createTableImpl( TableCreator* pTableCreator )
    {
        AFCStrategy::createTableImpl( pTableCreator );

        BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
        DEFINE_REAL_FIELD( q_ch4 )
        DEFINE_REAL_FIELD( k_ch4 )
        DEFINE_REAL_FIELD( q_ch4_tail )
        END_DEFINE_FIELD
    }

    void Gas_AFCStrategy::writeToTableImpl( DataWriter* pDataWriter )
    {
        AFCStrategy::writeToTableImpl( pDataWriter );

        BEGIN_WRITE_DATA( DataWriter, pDataWriter )
        WRITE_SIMPLE_REAL_DATA( q_ch4 )
        WRITE_SIMPLE_REAL_DATA( k_ch4 )
        WRITE_SIMPLE_REAL_DATA( q_ch4_tail )
        END_WRITE_DATA
    }
}