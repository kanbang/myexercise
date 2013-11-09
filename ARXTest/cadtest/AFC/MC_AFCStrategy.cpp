#include "StdAfx.h"
#include "MC_AFCStrategy.h"

namespace PJ
{
    MC_AFCStrategy::MC_AFCStrategy( const CString& tableName ) : AFCStrategy( tableName )
    {
    }

    double MC_AFCStrategy::getBasicQ() const
    {
        return 60 * getAverageKDJ() * height * 0.7 * reasonalVelocity;
    }

    void MC_AFCStrategy::setKDJ( double max_kdj, double min_kdj )
    {
        this->max_kdj = max_kdj;
        this->min_kdj = min_kdj;
    }

    double MC_AFCStrategy::getAverageKDJ() const
    {
        return ( max_kdj + min_kdj ) / 2.0;
    }

    void MC_AFCStrategy::setHeight( double height )
    {
        this->height = height;
    }

    void MC_AFCStrategy::setReasonalVelocity( double v )
    {
        this->reasonalVelocity = v;
    }

    void MC_AFCStrategy::setAdjustCoefficient( double k_height, double k_length, double k_temperature )
    {
        this->k_height = k_height;
        this->k_length = k_length;
        this->k_temperature = k_temperature;
    }

    double MC_AFCStrategy::caculate()
    {
        return getBasicQ() * k_height * k_length * k_temperature;
    }

    void MC_AFCStrategy::createTableImpl( TableCreator* pTableCreator )
    {
        AFCStrategy::createTableImpl( pTableCreator );

        BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
        DEFINE_REAL_FIELD( Q_basic ) // 基本风量
        DEFINE_REAL_FIELD( averageKDJ ) // 工作面控顶距
        DEFINE_REAL_FIELD( height )
        DEFINE_REAL_FIELD( reasonalVelocity )
        DEFINE_REAL_FIELD( k_height )
        DEFINE_REAL_FIELD( k_length )
        DEFINE_REAL_FIELD( k_temperature )
        END_DEFINE_FIELD
    }

    void MC_AFCStrategy::writeToTableImpl( DataWriter* pDataWriter )
    {
        AFCStrategy::writeToTableImpl( pDataWriter );

        BEGIN_WRITE_DATA( DataWriter, pDataWriter )
        WRITE_REAL_DATA( Q_basic, getBasicQ() )
        WRITE_REAL_DATA( averageKDJ, getAverageKDJ() )
        WRITE_SIMPLE_REAL_DATA( height )
        WRITE_SIMPLE_REAL_DATA( reasonalVelocity )
        WRITE_SIMPLE_REAL_DATA( k_height )
        WRITE_SIMPLE_REAL_DATA( k_length )
        WRITE_SIMPLE_REAL_DATA( k_temperature )
        END_WRITE_DATA
    }
}