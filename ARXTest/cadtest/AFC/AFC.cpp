#include "StdAfx.h"
#include "AFC.h"
#include "AFCStrategy.h"
#include "AFCStrategyComposite.h"
#include "AFCStrategyFactory.h"
#include "AFCTableNames.h"
#include "AFCResult.h"
#include "VelocityInfo.h"

namespace PJ
{
    AFC::AFC( const CString& tableName ): userQ( 0 ), name( _T( "${输入名称}" ) )
    {
        pAFC = new AFCStrategyComposite( tableName );
        pAFCStrategies =  new AFCStrategyCollection();
    }

    AFC::~AFC( void )
    {
        delete pAFCStrategies;
        delete pAFC;
    }

    void AFC::setName( const CString& name )
    {
        this->name = name;
    }

    void AFC::setUserQ( double q )
    {
        this->userQ = q;
    }

    void AFC::regAFCStrategy( const CString& tableName, AFCStrategy* pAFCStrategy )
    {
        AFCStrategyCollection::iterator itr = pAFCStrategies->find( tableName );

        if( itr == pAFCStrategies->end() )
        {
            pAFCStrategies->insert( AFCStrategyCollection::value_type( tableName, pAFCStrategy ) );
        }
        else
        {
            delete itr->second; // 删除原先的AFCStrategy对象
            itr->second = pAFCStrategy;
            //afc_strategies[tableName] = pAFCStrategy;
        }
    }

    void AFC::prepare()
    {
        for( AFCStrategyCollection::iterator itr = pAFCStrategies->begin(); itr != pAFCStrategies->end(); itr++ )
        {
            if( itr->second == 0 )
            {
                pAFC->regAFCStrategy( AFCStrategyFactory().createNull_AFCStrategy( itr->first ) );
            }
            else
            {
                pAFC->regAFCStrategy( itr->second );
            }
        }
    }

    void AFC::clean()
    {
        pAFC->clearAllAFCStrategies();

        for( AFCStrategyCollection::iterator itr = pAFCStrategies->begin(); itr != pAFCStrategies->end(); itr++ )
        {
            delete itr->second;
            //afc_strategies[itr->first] = 0;
        }
        pAFCStrategies->clear();
    }

    double AFC::doAFC( TableCreator* pTableCreator, DataWriter* pDataWriter )
    {
        if( pAFC == 0 ) return 0;
        prepare();

        AFCResult afc_result( AFC_RESULT_TABLE_NAME );
        afc_result.regAFCStrategyComposite( name, userQ, pAFC );

        afc_result.createTable( pTableCreator );
        afc_result.writeToTable( pDataWriter );

        double maxQ = afc_result.getMaxQ();

        clean();
        return maxQ;
    }

    WS_AFC::WS_AFC() : AFC( WS_AFC_TABLE_NAME )
    {
        regAFCStrategy( MC_AFC_TABLE_NAME, 0 );
        regAFCStrategy( GAS_AFC_TABLE_NAME, 0 );
        regAFCStrategy( TVE_AFC_TABLE_NAME, 0 );
        regAFCStrategy( MAX_PEOPLE_NUM_AFC_CHECK_TABLE_NAME, 0 );
        regAFCStrategy( MAX_WEIGHT_AFC_CHECK_TABLE_NAME, 0 );
        regAFCStrategy( VE_AFC_CHECK_TABLE_NAME, 0 );
    }

    void WS_AFC::initMC_AFCStrategy( double max_kdj, double min_kdj, double height, double reasonalVelocity, double k_height, double k_length, double k_temperature )
    {
        regAFCStrategy( MC_AFC_TABLE_NAME,
                        AFCStrategyFactory().createMC_AFCStrategy( max_kdj, min_kdj, height, reasonalVelocity, k_height, k_length, k_temperature ) );
    }

    void WS_AFC::initGas_AFCStrategy( double q_ch4, double k_ch4, double q_ch4_tail )
    {
        regAFCStrategy( GAS_AFC_TABLE_NAME,
                        AFCStrategyFactory().createGas_AFCStrategy( q_ch4, k_ch4, q_ch4_tail ) );
    }

    void WS_AFC::initTve_AFCStrategy( double reasonalVelocity, double area )
    {
        regAFCStrategy( TVE_AFC_TABLE_NAME,
                        AFCStrategyFactory().createTve_AFCStrategy( reasonalVelocity, area ) );
    }

    void WS_AFC::initMaxPeopleNum_AFC_CheckStrategy( int maxPeopleNum )
    {
        regAFCStrategy( MAX_PEOPLE_NUM_AFC_CHECK_TABLE_NAME,
                        AFCStrategyFactory().createMaxPeopleNum_AFC_CheckStrategy( maxPeopleNum ) );
    }

    void WS_AFC::initMaxWeight_AFC_CheckStrategy( double maxWeight )
    {
        regAFCStrategy( MAX_WEIGHT_AFC_CHECK_TABLE_NAME,
                        AFCStrategyFactory().createMaxWeight_AFC_CheckStrategy( maxWeight ) );
    }

    void WS_AFC::initVE_AFC_CheckStrategy( double area, bool hasGasTunnel )
    {
        MIN_VELOCITY V_min = NORMAL_MIN_VELOCITY;
        MAX_VELOCITY V_max = NORMAL_MAX_VELOCITY;

        if( hasGasTunnel )
        {
            V_min = LARGE_MIN_VELOCITY;
        }

        VelocityCheckType vct = VCT_WS_NORMAL;
        if( hasGasTunnel ) vct = VCT_WS_GASTUNNEL;

        regAFCStrategy( VE_AFC_CHECK_TABLE_NAME,
                        AFCStrategyFactory().createVE_AFC_CheckStrategy( area, V_min, V_max, vct ) );
    }

    WS_AFC::~WS_AFC()
    {

    }

    TWS_AFC::TWS_AFC() : AFC( TWS_AFC_TABLE_NAME )
    {
        regAFCStrategy( GAS_AFC_TABLE_NAME, 0 );
        regAFCStrategy( LOCAL_FAN_AFC_TABLE_NAME, 0 );
        regAFCStrategy( MAX_PEOPLE_NUM_AFC_CHECK_TABLE_NAME, 0 );
        regAFCStrategy( MAX_WEIGHT_AFC_CHECK_TABLE_NAME, 0 );
        regAFCStrategy( VE_AFC_CHECK_TABLE_NAME, 0 );
    }

    void TWS_AFC::initGas_AFCStrategy( double q_ch4, double k_ch4 )
    {
        regAFCStrategy( GAS_AFC_TABLE_NAME,
                        AFCStrategyFactory().createGas_AFCStrategy( q_ch4, k_ch4 ) );
    }

    void TWS_AFC::initLocalFan_AFCStrategy( double area, int fanNum, double q_fan, bool isRock )
    {
        regAFCStrategy( LOCAL_FAN_AFC_TABLE_NAME,
                        AFCStrategyFactory().createLocalFan_AFCStrategy( area, fanNum, q_fan, isRock ) );
    }

    void TWS_AFC::initMaxPeopleNum_AFC_CheckStrategy( int maxPeopleNum )
    {
        regAFCStrategy( MAX_PEOPLE_NUM_AFC_CHECK_TABLE_NAME,
                        AFCStrategyFactory().createMaxPeopleNum_AFC_CheckStrategy( maxPeopleNum ) );
    }

    void TWS_AFC::initMaxWeight_AFC_CheckStrategy( double maxWeight )
    {
        regAFCStrategy( MAX_WEIGHT_AFC_CHECK_TABLE_NAME,
                        AFCStrategyFactory().createMaxWeight_AFC_CheckStrategy( maxWeight ) );
    }

    void TWS_AFC::initVE_AFC_CheckStrategy( double area, bool isRock )
    {
        MIN_VELOCITY V_min = NORMAL_MIN_VELOCITY;
        MAX_VELOCITY V_max = NORMAL_MAX_VELOCITY;

        if( isRock )
        {
            V_min = SMALL_MIN_VELOCITY;
        }

        VelocityCheckType vct = VCT_COAL;
        if( isRock ) vct = VCT_ROCK;

        regAFCStrategy( VE_AFC_CHECK_TABLE_NAME,
                        AFCStrategyFactory().createVE_AFC_CheckStrategy( area, V_min, V_max, vct ) );
    }

    TWS_AFC::~TWS_AFC()
    {

    }
    DHC_AFC::DHC_AFC() : AFC( DHC_AFC_TABLE_NAME )
    {
        regAFCStrategy( UEM_AFC_TABLE_NAME, 0 );
        regAFCStrategy( PCR_AFC_TABLE_NAME, 0 );
        regAFCStrategy( MEC_AFC_TABLE_NAME, 0 );
        regAFCStrategy( CONST_MINQ_AFC_CHECK_TABLE_NAME, 0 );
    }

    void DHC_AFC::initUEM_AFCStrategy( double V )
    {
        regAFCStrategy( UEM_AFC_TABLE_NAME,
                        AFCStrategyFactory().createUEM_AFCStrategy( V ) );
    }

    void DHC_AFC::initPCR_AFCStrategy( double q_h2 )
    {
        regAFCStrategy( PCR_AFC_TABLE_NAME,
                        AFCStrategyFactory().createPCR_AFCStrategy( q_h2 ) );
    }

    void DHC_AFC::initLargeMEC_AFCStrategy( double power, double theta, double delta_t )
    {
        regAFCStrategy( MEC_AFC_TABLE_NAME,
                        AFCStrategyFactory().createLargeMEC_AFCStrategy( power, theta, delta_t ) );
    }

    void DHC_AFC::initSmallMEC_AFCStrategy( double power, double q_def )
    {
        regAFCStrategy( MEC_AFC_TABLE_NAME,
                        AFCStrategyFactory().createSmallMEC_AFCStrategy( power, q_def ) );
    }

    void DHC_AFC::initConstMinQ_AFC_CheckStrategy()
    {
        regAFCStrategy( CONST_MINQ_AFC_CHECK_TABLE_NAME,
                        AFCStrategyFactory().createConstMinQ_AFC_CheckStrategy() );
    }

    DHC_AFC::~DHC_AFC()
    {

    }
    OT_AFC::OT_AFC() : AFC( OT_AFC_TABLE_NAME )
    {
        regAFCStrategy( OT_GAS_AFC_TABLE_NAME, 0 );
        regAFCStrategy( VE_AFC_CHECK_TABLE_NAME, 0 );
        regAFCStrategy( RTC_AFC_TABLE_NAME, 0 );
    }

    void OT_AFC::initOT_Gas_AFCStrategy( double q_ch4_max, CoalMineGasInfo gasInfo, TunnelMaterial tm )
    {
        regAFCStrategy( OT_GAS_AFC_TABLE_NAME,
                        AFCStrategyFactory().createOT_Gas_AFCStrategy( q_ch4_max, gasInfo, tm ) );
    }

    void OT_AFC::initVE_AFC_CheckStrategy( double area, bool isRock )
    {
        MIN_VELOCITY V_min = NORMAL_MIN_VELOCITY;
        MAX_VELOCITY V_max = NO_LIMIT_MAX_VELOCITY;

        if( isRock )
        {
            V_min = SMALL_MIN_VELOCITY;
        }

        VelocityCheckType vct = VCT_COAL;
        if( isRock ) vct = VCT_ROCK;

        regAFCStrategy( VE_AFC_CHECK_TABLE_NAME,
                        AFCStrategyFactory().createVE_AFC_CheckStrategy( area, V_min, V_max, vct ) );
    }

    void OT_AFC::initEl_VE_AFC_CheckStrategy( double area )
    {
        MIN_VELOCITY V_min = MORE_MIN_VELOCITY;
        MAX_VELOCITY V_max = NO_LIMIT_MAX_VELOCITY;

        VelocityCheckType vct = VCT_EL;

        regAFCStrategy( VE_AFC_CHECK_TABLE_NAME,
                        AFCStrategyFactory().createVE_AFC_CheckStrategy( area, V_min, V_max, vct ) );
    }

    void OT_AFC::initRTC_AFCStrategy( int num, double power )
    {
        regAFCStrategy( RTC_AFC_TABLE_NAME,
                        AFCStrategyFactory().createRTC_AFCStrategy( num, power ) );
    }

    OT_AFC::~OT_AFC()
    {

    }
}