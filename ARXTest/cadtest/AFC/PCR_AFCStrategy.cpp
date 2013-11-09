#include "StdAfx.h"
#include "PCR_AFCStrategy.h"

namespace PJ
{
    PCR_AFCStrategy::PCR_AFCStrategy( const CString& tableName ) : AFCStrategy( tableName )
    {
    }

    void PCR_AFCStrategy::setQ_H2( double q )
    {
        this->q_h2 = q;
    }

    double PCR_AFCStrategy::caculate()
    {
        return 200 * q_h2;
    }

    void PCR_AFCStrategy::createTableImpl( TableCreator* pTableCreator )
    {
        AFCStrategy::createTableImpl( pTableCreator );

        BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
        DEFINE_REAL_FIELD( q_h2 )
        END_DEFINE_FIELD
    }

    void PCR_AFCStrategy::writeToTableImpl( DataWriter* pDataWriter )
    {
        AFCStrategy::writeToTableImpl( pDataWriter );

        BEGIN_WRITE_DATA( DataWriter, pDataWriter )
        WRITE_SIMPLE_REAL_DATA( q_h2 )
        END_WRITE_DATA
    }
}