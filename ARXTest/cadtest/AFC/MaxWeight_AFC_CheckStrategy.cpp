#include "StdAfx.h"
#include "MaxWeight_AFC_CheckStrategy.h"

namespace PJ
{
    MaxWeight_AFC_CheckStrategy::MaxWeight_AFC_CheckStrategy( const CString& tableName ) : AFC_CheckStrategy( tableName )
    {
    }

    void MaxWeight_AFC_CheckStrategy::setMaxWeight( double weight )
    {
        this->maxWeight = weight;
    }

    double MaxWeight_AFC_CheckStrategy::minQ()
    {
        return 25 * maxWeight;
    }

    void MaxWeight_AFC_CheckStrategy::createTableImpl( TableCreator* pTableCreator )
    {
        AFC_CheckStrategy::createTableImpl( pTableCreator );

        BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
        DEFINE_REAL_FIELD( maxWeight )
        END_DEFINE_FIELD
    }

    void MaxWeight_AFC_CheckStrategy::writeToTableImpl( DataWriter* pDataWriter )
    {
        AFC_CheckStrategy::writeToTableImpl( pDataWriter );

        BEGIN_WRITE_DATA( DataWriter, pDataWriter )
        WRITE_SIMPLE_REAL_DATA( maxWeight )
        END_WRITE_DATA
    }
}