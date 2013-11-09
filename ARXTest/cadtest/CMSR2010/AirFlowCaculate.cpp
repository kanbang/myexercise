#include "StdAfx.h"
#include "AirFlowCaculate.h"

#include "../MineGE/DrawHelper.h"
#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"
#include "../AFC/AFC.h"
using namespace PJ;

#define DEF_POWER 300

static CDaoDatabase* CreateDB( const CString& dbName, bool openExist = false )
{
    CDaoDatabase* pDB = new CDaoDatabase();
    CFileFind ff;
    if( ff.FindFile( dbName ) )
    {
        if( openExist )
        {
            pDB->Open( dbName );
        }
        else
        {
            CFile::Remove( dbName );
            pDB->Create( dbName );
        }
    }
    else
    {
        pDB->Create( dbName );
    }
    return pDB;
}

void AirFlowCaculate::setMineGasState( bool isHigh )
{
    highGasMine = isHigh;
}

void AirFlowCaculate::doAFC( const CString& path, AcArray<AFCDataLink*>& ts )
{
    CDaoDatabase* pDB = CreateDB( path );
    TableCreator* pTableCreator = new TableCreator( pDB );
    DataWriter* pDataWriter = new DataWriter( pDB );

    doWsAFC( pTableCreator, pDataWriter );
    ts.append( ds );

    doTwsAFC( pTableCreator, pDataWriter );
    ts.append( ds );

    doDhcAFC1( pTableCreator, pDataWriter );
    ts.append( ds );

    doDhcAFC2( pTableCreator, pDataWriter );
    ts.append( ds );

    doDhcAFC3( pTableCreator, pDataWriter );
    ts.append( ds );

    doOtAFC( pTableCreator, pDataWriter );
    ts.append( ds );

    delete pTableCreator;
    delete pDataWriter;
    pDB->Close();
    delete pDB;

    if( ts.isEmpty() )
    {
        acutPrintf( _T( "\n删除风量计算结果数据库..." ) );
        CFile::Remove( path );
    }
}

void AirFlowCaculate::doWsAFC( TableCreator* pTableCreator, DataWriter* pDataWriter )
{
    clean();
    init_WSModel();

    int len = ds.length();
    for( int i = 0; i < len; i++ )
    {
        WS_AFC ws_afc;
        WS_AFCDataLink* pWs = ( WS_AFCDataLink* )ds[i];
        acutPrintf( _T( "\n%s 进行风量计算" ), pWs->name );
        ws_afc.setName( pWs->name );
        ws_afc.setUserQ( pWs->userQ );
        if( highGasMine )
        {
            ws_afc.initGas_AFCStrategy( pWs->q_ch4, pWs->k_ch4, pWs->q_ch4_tail );
        }
        else
        {
            ws_afc.initMC_AFCStrategy( pWs->max_kdj, pWs->min_kdj, pWs->height,
                                       pWs->reasonalVelocity, pWs->k_height,
                                       pWs->k_length, pWs->k_temperature );
        }
        ws_afc.initMaxPeopleNum_AFC_CheckStrategy( pWs->maxPeopleNum );
        ws_afc.initMaxWeight_AFC_CheckStrategy( pWs->maxWeight );
        ws_afc.initTve_AFCStrategy( pWs->reasonalVelocity, pWs->area );
        ws_afc.initVE_AFC_CheckStrategy( pWs->area, !( pWs->q_ch4_tail < 0.001 ) );
        pWs->maxQ = ws_afc.doAFC( pTableCreator, pDataWriter );
    }
}

void AirFlowCaculate::doTwsAFC( TableCreator* pTableCreator, DataWriter* pDataWriter )
{
    clean();
    init_TWSModel();

    int len = ds.length();
    for( int i = 0; i < len; i++ )
    {
        TWS_AFC tws_afc;
        TWS_AFCDataLink* pTws = ( TWS_AFCDataLink* )ds[i];
        tws_afc.setName( pTws->name );
        tws_afc.setUserQ( pTws->userQ );
        tws_afc.initGas_AFCStrategy( pTws->q_ch4, pTws->k_ch4 );
        tws_afc.initLocalFan_AFCStrategy( pTws->area, pTws->fanNum, ( pTws->tm == ROCK ) );
        tws_afc.initMaxPeopleNum_AFC_CheckStrategy( pTws->maxPeopleNum );
        tws_afc.initMaxWeight_AFC_CheckStrategy( pTws->maxWeight );
        tws_afc.initVE_AFC_CheckStrategy( pTws->area, ( pTws->tm == ROCK ) );
        pTws->maxQ = tws_afc.doAFC( pTableCreator, pDataWriter );
    }
}

void AirFlowCaculate::doDhcAFC1( TableCreator* pTableCreator, DataWriter* pDataWriter )
{
    // 1、爆炸材料库风量计算
    clean();
    init_StorageModel1();
    int len = ds.length();
    for( int i = 0; i < len; i++ )
    {
        DHC_AFC dhc_afc;
        Powder_AFCDataLink* pPm = ( Powder_AFCDataLink* )ds[i];
        dhc_afc.setName( pPm->name );
        dhc_afc.setUserQ( pPm->userQ );
        dhc_afc.initUEM_AFCStrategy( pPm->V );
        dhc_afc.initConstMinQ_AFC_CheckStrategy();
        pPm->maxQ = dhc_afc.doAFC( pTableCreator, pDataWriter );
    }
}

void AirFlowCaculate::doDhcAFC2( TableCreator* pTableCreator, DataWriter* pDataWriter )
{
    // 2、充电室风量计算
    clean();
    init_StorageModel2();
    int len = ds.length();
    for( int i = 0; i < len; i++ )
    {
        DHC_AFC dhc_afc;
        Charge_AFCDataLink* pCm = ( Charge_AFCDataLink* )ds[i];
        dhc_afc.setName( pCm->name );
        dhc_afc.setUserQ( pCm->userQ );
        dhc_afc.initPCR_AFCStrategy( pCm->q_h2 );
        dhc_afc.initConstMinQ_AFC_CheckStrategy();
        dhc_afc.doAFC( pTableCreator, pDataWriter );
    }
}

void AirFlowCaculate::doDhcAFC3( TableCreator* pTableCreator, DataWriter* pDataWriter )
{
    // 3、机电硐室风量计算
    clean();
    init_StorageModel3();
    int len = ds.length();
    for( int i = 0; i < len; i++ )
    {
        DHC_AFC dhc_afc;
        Machine_AFCDataLink* pMm = ( Machine_AFCDataLink* )ds[i];
        dhc_afc.setName( pMm->name );
        dhc_afc.setUserQ( pMm->userQ );
        if( pMm->power > DEF_POWER )
        {
            // 大型机电硐室
            dhc_afc.initLargeMEC_AFCStrategy( pMm->power, pMm->theta, pMm->delta_t );
        }
        else
        {
            // 小型机电硐室
            dhc_afc.initSmallMEC_AFCStrategy( pMm->power, 80 ); // 取80
        }
        pMm->maxQ = dhc_afc.doAFC( pTableCreator, pDataWriter );
    }
}

void AirFlowCaculate::doOtAFC( TableCreator* pTableCreator, DataWriter* pDataWriter )
{
    clean();
    init_OTModel();

    int len = ds.length();
    for( int i = 0; i < len; i++ )
    {
        OT_AFC ot_afc;
        Tunnel_AFCDataLink* pTm = ( Tunnel_AFCDataLink* )ds[i];
        ot_afc.setName( pTm->name );
        ot_afc.setUserQ( pTm->userQ );
        if( ( pTm->an & AN_1 ) > 0 )
        {
            CoalMineGasInfo cgi = ( highGasMine ? ( LOW_GAS ) : ( HIGH_GAS ) );
            ot_afc.initOT_Gas_AFCStrategy( pTm->q_ch4, cgi, ( TunnelMaterial )pTm->tm );
        }
        if( ( pTm->an & AN_2 ) > 0 )
        {
            ot_afc.initVE_AFC_CheckStrategy( pTm->area, ( pTm->tm == ROCK ) );
        }
        if( ( pTm->an & AN_3 ) > 0 )
        {
            ot_afc.initEl_VE_AFC_CheckStrategy( pTm->area );
        }

        if( ( pTm->an & AN_4 ) > 0 )
        {
            ot_afc.initRTC_AFCStrategy( pTm->num, pTm->power );
        }
        pTm->maxQ = ot_afc.doAFC( pTableCreator, pDataWriter );
    }
}

void AirFlowCaculate::init_WSModel()
{
    // 1、查找所有的工作面图元
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "WorkSurface" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2、关联
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        WS_AFCDataLink* pWsm = new WS_AFCDataLink();
        pWsm->setDataSource( objIds[i] );
        if( !pWsm->updateData( false ) )
        {
            // 清理,并退出
            delete pWsm;
            clean();
            break;
        }
        ds.append( pWsm );
    }
}

void AirFlowCaculate::init_TWSModel()
{
    // 1、查找所有的掘进工作面图元
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "TTunnel" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2、关联
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TWS_AFCDataLink* pTwsm = new TWS_AFCDataLink();
        pTwsm->setDataSource( objIds[i] );
        if( !pTwsm->updateData( false ) )
        {
            // 清理,并退出
            delete pTwsm;
            clean();
            break;
        }
        ds.append( pTwsm );
    }
}

void AirFlowCaculate::init_StorageModel1()
{
    // 1、查找所有的爆炸材料库图元
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "PowderStorage" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2、关联
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        Powder_AFCDataLink* pPm = new Powder_AFCDataLink();
        pPm->setDataSource( objIds[i] );
        if( !pPm->updateData( false ) )
        {
            // 清理,并退出
            delete pPm;
            clean();
            break;
        }
        ds.append( pPm );
    }
}

void AirFlowCaculate::init_StorageModel2()
{
    // 1、查找所有的充电室图元
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "ChargeRoom" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2、关联
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        Charge_AFCDataLink* pCm = new Charge_AFCDataLink();
        pCm->setDataSource( objIds[i] );
        if( !pCm->updateData( false ) )
        {
            // 清理,并退出
            delete pCm;
            clean();
            break;
        }
        ds.append( pCm );
    }
}

void AirFlowCaculate::init_StorageModel3()
{
    // 1、查找所有的机电硐室图元
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "MachineRoom" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2、关联
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        Machine_AFCDataLink* pMm = new Machine_AFCDataLink();
        pMm->setDataSource( objIds[i] );
        if( !pMm->updateData( false ) )
        {
            // 清理,并退出
            delete pMm;
            clean();
            break;
        }
        ds.append( pMm );
    }
}

void AirFlowCaculate::init_OTModel()
{
    // 1、查找所有的巷道图元
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "Tunnel" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2、关联
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        Tunnel_AFCDataLink* pTm = new Tunnel_AFCDataLink();
        pTm->setDataSource( objIds[i] );
        if( !pTm->updateData( false ) ) // 提取数据失败
        {
            // 清理,并退出
            delete pTm;
            clean();
            break;
        }

        int ft = ( pTm->ft % 6 ); // 范围:0-5

        // 不需要单独配风，且无架线电车和胶轮车运行
        // 不进行风量计算
        if( !pTm->needAir && ft != FT_ELECTRIC && ft != FT_DIESEL )
        {
            delete pTm;
            continue;
        }

        pTm->an = ( AFC_NUM )( AN_1 | AN_2 ); // 默认需要进行瓦斯计算和风速验算
        if( ft == FT_ELECTRIC ) pTm->an = ( AFC_NUM )( pTm->an | AN_3 );
        if( ft == FT_DIESEL ) pTm->an = ( AFC_NUM )( pTm->an | AN_4 );

        ds.append( pTm );
    }
}

// 不能delete，因为ds集合需要给外部的评价程序使用
void AirFlowCaculate::clean()
{
    /*if(ds.isEmpty()) return;
    int len = ds.length();
    for(int i=0;i<len;i++)
    {
    	delete ds[i];
    }*/
    ds.removeAll();
}
