#include "StdAfx.h"
#include "MEC_AFCStrategy.h"

namespace PJ
{
    MEC_AFCStrategy::MEC_AFCStrategy( const CString& tableName ) : AFCStrategy( tableName )
    {
        isLargeMEC = true;
        rho = 1.20;
        c_p = 1.0006;
        theta = 0;
        delta_t = 0;
        power = 0;
    }

    void MEC_AFCStrategy::setPower( double power )
    {
        this->power = power;
    }

    void MEC_AFCStrategy::setTheta( double theta )
    {
        this->theta = theta;
    }

    void MEC_AFCStrategy::setDeltaT( double delta_t )
    {
        this->delta_t = delta_t;
    }

    void MEC_AFCStrategy::setMECSize( bool isLargeMEC )
    {
        this->isLargeMEC = isLargeMEC;
    }

    void MEC_AFCStrategy::setDefQ( double q )
    {
        this->q_def = q;
    }

    double MEC_AFCStrategy::caculate()
    {
        if( isLargeMEC )
        {
            return 3600 * power * theta / ( rho * c_p * 60 * delta_t );
        }
        else
            return q_def;
    }

    void MEC_AFCStrategy::createTableImpl( TableCreator* pTableCreator )
    {
        AFCStrategy::createTableImpl( pTableCreator );

        BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
        DEFINE_BOOL_FIELD( isLargeMEC )
        DEFINE_REAL_FIELD( power )
        DEFINE_REAL_FIELD( theta )
        DEFINE_REAL_FIELD( rho )
        DEFINE_REAL_FIELD( c_p )
        DEFINE_REAL_FIELD( delta_t )
        END_DEFINE_FIELD
    }

    void MEC_AFCStrategy::writeToTableImpl( DataWriter* pDataWriter )
    {
        AFCStrategy::writeToTableImpl( pDataWriter );

        BEGIN_WRITE_DATA( DataWriter, pDataWriter )
        WRITE_SIMPLE_BOOL_DATA( isLargeMEC )
        WRITE_SIMPLE_REAL_DATA( power )
        WRITE_SIMPLE_REAL_DATA( theta )
        WRITE_SIMPLE_REAL_DATA( rho )
        WRITE_SIMPLE_REAL_DATA( c_p )
        WRITE_SIMPLE_REAL_DATA( delta_t )
        END_WRITE_DATA
    }
}