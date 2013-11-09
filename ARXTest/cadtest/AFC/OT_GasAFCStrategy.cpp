#include "StdAfx.h"
#include "OT_GasAFCStrategy.h"

namespace PJ
{
    OT_GasAFCStrategy::OT_GasAFCStrategy( const CString& tableName ) : AFCStrategy( tableName )
    {
    }

    void OT_GasAFCStrategy::setQ_CH4_Max( double q )
    {
        this->q_ch4_max = q;
    }

    void OT_GasAFCStrategy::setInfo( CoalMineGasInfo gasInfo, TunnelMaterial tm )
    {
        if( gasInfo == LOW_GAS && tm == ROCK ) k_ch4 = 1.2;

        k_ch4 = 1.3;
    }

    double OT_GasAFCStrategy::caculate()
    {
        return 100 * q_ch4_max * k_ch4;
    }

    void OT_GasAFCStrategy::createTableImpl( TableCreator* pTableCreator )
    {
        AFCStrategy::createTableImpl( pTableCreator );

        BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
        DEFINE_REAL_FIELD( q_ch4_max )
        DEFINE_REAL_FIELD( k_ch4 )
        END_DEFINE_FIELD
    }

    void OT_GasAFCStrategy::writeToTableImpl( DataWriter* pDataWriter )
    {
        AFCStrategy::writeToTableImpl( pDataWriter );

        BEGIN_WRITE_DATA( DataWriter, pDataWriter )
        WRITE_SIMPLE_REAL_DATA( q_ch4_max )
        WRITE_SIMPLE_REAL_DATA( k_ch4 )
        END_WRITE_DATA
    }
}