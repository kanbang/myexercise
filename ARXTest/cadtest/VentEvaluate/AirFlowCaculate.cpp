#include "StdAfx.h"
#include "AirFlowCaculate.h"

#include "../MineGE/DrawHelper.h"

#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"

#include "../AFC/AFC.h"
using namespace PJ;

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

void AirFlowCaculate::test( const CString& path )
{
    CDaoDatabase* pDB = CreateDB( path );
    TableCreator* pTableCreator = new TableCreator( pDB );
    DataWriter* pDataWriter = new DataWriter( pDB );

    ws_afc_test( pTableCreator, pDataWriter );
    tws_afc_test( pTableCreator, pDataWriter );
    dhc_afc_test( pTableCreator, pDataWriter );
    ot_afc_test( pTableCreator, pDataWriter );

    delete pTableCreator;
    delete pDataWriter;
    pDB->Close();
    delete pDB;
}

void AirFlowCaculate::ws_afc_test( TableCreator* pTableCreator, DataWriter* pDataWriter )
{
    clean();
    init_WSModel();

    WS_AFC ws_afc;
    int len = mc.length();
    for( int i = 0; i < len; i++ )
    {
        WS_AFCDataLink* pWs = ( WS_AFCDataLink* )mc[i];
        if( highGasMine )
        {
            ws_afc.initGas_AFCStrategy( pWs->q_ch4, pWs->k_ch4, pWs->q_ch4_tail );
        }
        else
        {
            ws_afc.initMC_AFCStrategy( pWs->max_kdj, pWs->min_kdj, pWs->height, pWs->reasonalVelocity, pWs->k_height, pWs->k_length, pWs->k_temperature );
        }
        ws_afc.initMaxPeopleNum_AFC_CheckStrategy( pWs->maxPeopleNum );
        ws_afc.initMaxWeight_AFC_CheckStrategy( pWs->maxWeight );
        ws_afc.initTve_AFCStrategy( pWs->reasonalVelocity, pWs->area );
        ws_afc.initVE_AFC_CheckStrategy( pWs->area, !( pWs->q_ch4_tail < 0.001 ) );
        ws_afc.doAFC( pTableCreator, pDataWriter );
    }
}

void AirFlowCaculate::tws_afc_test( TableCreator* pTableCreator, DataWriter* pDataWriter )
{
    clean();
    init_TWSModel();

    TWS_AFC tws_afc;
    int len = mc.length();
    for( int i = 0; i < len; i++ )
    {
        TWS_AFCDataLink* pTws = ( TWS_AFCDataLink* )mc[i];
        tws_afc.initGas_AFCStrategy( pTws->q_ch4, pTws->k_ch4 );
        tws_afc.initLocalFan_AFCStrategy( pTws->area, pTws->fanNum, ( pTws->tunnelMaterial == ROCK ) ); // 1��ʾ����
        tws_afc.initMaxPeopleNum_AFC_CheckStrategy( pTws->maxPeopleNum );
        tws_afc.initMaxWeight_AFC_CheckStrategy( pTws->maxWeight );
        tws_afc.initVE_AFC_CheckStrategy( pTws->area, ( pTws->tunnelMaterial == ROCK ) );
        tws_afc.doAFC( pTableCreator, pDataWriter );
    }
}

void AirFlowCaculate::dhc_afc_test( TableCreator* pTableCreator, DataWriter* pDataWriter )
{
    DHC_AFC dhc_afc;

    // 1����ը���Ͽ��������
    clean();
    init_StorageModel1();
    int len = mc.length();
    for( int i = 0; i < len; i++ )
    {
        Powder_AFCDataLink* pPm = ( Powder_AFCDataLink* )mc[i];
        dhc_afc.initUEM_AFCStrategy( pPm->V );
        dhc_afc.initConstMinQ_AFC_CheckStrategy();
        dhc_afc.doAFC( pTableCreator, pDataWriter );
    }

    // 2������ҷ�������
    clean();
    init_StorageModel2();
    len = mc.length();
    for( int i = 0; i < len; i++ )
    {
        Charge_AFCDataLink* pCm = ( Charge_AFCDataLink* )mc[i];
        dhc_afc.initPCR_AFCStrategy( pCm->q_h2 );
        dhc_afc.initConstMinQ_AFC_CheckStrategy();
        dhc_afc.doAFC( pTableCreator, pDataWriter );
    }

    // 3���������ҷ�������
    clean();
    init_StorageModel3();
    len = mc.length();
    for( int i = 0; i < len; i++ )
    {
        Machine_AFCDataLink* pMm = ( Machine_AFCDataLink* )mc[i];
        if( pMm->power > DEF_POWER )
        {
            // ���ͻ�������
            dhc_afc.initLargeMEC_AFCStrategy( pMm->power, pMm->theta, pMm->delta_t );
        }
        else
        {
            // С�ͻ�������
            dhc_afc.initSmallMEC_AFCStrategy( pMm->power, 80 ); // ȡ80
        }
        dhc_afc.doAFC( pTableCreator, pDataWriter );
    }
}

void AirFlowCaculate::ot_afc_test( TableCreator* pTableCreator, DataWriter* pDataWriter )
{
    clean();
    init_OTModel();
    if( mc.isEmpty() ) return;

    OT_AFC ot_afc;
    int len = mc.length();
    for( int i = 0; i < len; i++ )
    {
        Tunnel_AFCDataLink* pTm = ( Tunnel_AFCDataLink* )mc[i];
        if( ( pTm->an & AN_1 ) > 0 )
        {
            CoalMineGasInfo cgi = ( highGasMine ? ( LOW_GAS ) : ( HIGH_GAS ) );
            ot_afc.initOT_Gas_AFCStrategy( pTm->q_ch4, cgi, ( TunnelMaterial )pTm->tunnelMaterial );
        }
        if( ( pTm->an & AN_2 ) > 0 )
        {
            ot_afc.initVE_AFC_CheckStrategy( pTm->area, ( pTm->tunnelMaterial == ROCK ) );
        }
        if( ( pTm->an & AN_3 ) > 0 )
        {
            ot_afc.initEl_VE_AFC_CheckStrategy( pTm->area );
        }

        if( pTm->pRTCModel != 0 )
        {
            ot_afc.initRTC_AFCStrategy( pTm->pRTCModel->num, pTm->pRTCModel->power );
        }
        ot_afc.doAFC( pTableCreator, pDataWriter );
    }
}

void AirFlowCaculate::init_WSModel()
{
    // 1���������еĹ�����ͼԪ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "WorkSurface" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        WS_AFCDataLink* pWsm = new WS_AFCDataLink();
        pWsm->setDataSource( objIds[i] );
        if( !pWsm->updateData( false ) )
        {
            // ����,���˳�
            delete pWsm;
            clean();
            break;
        }
        mc.append( pWsm );
    }
}

void AirFlowCaculate::init_TWSModel()
{
    // 1���������еľ�����ͼԪ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "TTunnel" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TWS_AFCDataLink* pTwsm = new TWS_AFCDataLink();
        pTwsm->setDataSource( objIds[i] );
        if( !pTwsm->updateData( false ) )
        {
            // ����,���˳�
            delete pTwsm;
            clean();
            break;
        }
        mc.append( pTwsm );
    }
}

void AirFlowCaculate::init_StorageModel1()
{
    // 1���������еı�ը���Ͽ�ͼԪ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "PowderStorage" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        Powder_AFCDataLink* pPm = new Powder_AFCDataLink();
        pPm->setDataSource( objIds[i] );
        if( !pPm->updateData( false ) )
        {
            // ����,���˳�
            delete pPm;
            clean();
            break;
        }
        mc.append( pPm );
    }
}

void AirFlowCaculate::init_StorageModel2()
{
    // 1���������еĳ����ͼԪ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "ChargeRoom" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        Charge_AFCDataLink* pCm = new Charge_AFCDataLink();
        pCm->setDataSource( objIds[i] );
        if( !pCm->updateData( false ) )
        {
            // ����,���˳�
            delete pCm;
            clean();
            break;
        }
        mc.append( pCm );
    }
}

void AirFlowCaculate::init_StorageModel3()
{
    // 1���������еĻ�������ͼԪ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "MachineRoom" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        Machine_AFCDataLink* pMm = new Machine_AFCDataLink();
        pMm->setDataSource( objIds[i] );
        if( !pMm->updateData( false ) )
        {
            // ����,���˳�
            delete pMm;
            clean();
            break;
        }
        mc.append( pMm );
    }
}

void AirFlowCaculate::init_OTModel()
{
    // 1���������е����ͼԪ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "Tunnel" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        Tunnel_AFCDataLink* pTm = new Tunnel_AFCDataLink();
        pTm->setDataSource( objIds[i] );
        if( !pTm->updateData( false ) )
        {
            // ����,���˳�
            delete pTm;
            clean();
            break;
        }

        // ����Ҫ������磬��˲���Ҫ���з�������
        if( !pTm->needAir )
        {
            delete pTm;
            continue;
        }

        pTm->an = ( AFC_NUM )( AN_1 | AN_2 ); // Ĭ����Ҫ������˹����ͷ�������

        AcDbObjectIdArray cars; // ���Ҽ��ߵ糵
        DrawHelper::GetTagGEById2( objIds[i], _T( "ElectricWireCar" ), cars );
        if( !cars.isEmpty() ) pTm->an = ( AFC_NUM )( pTm->an | AN_3 );

        AcDbObjectIdArray diesels; // ���ҽ��ֳ�
        DrawHelper::GetTagGEById2( objIds[i], _T( "Diesel" ), diesels );

        if( !diesels.isEmpty() )
        {
            RTC_AFCDataLink* pRTC = new RTC_AFCDataLink();
            pRTC->setDataSource( diesels[0] ); // ����:һ�����ֻ��һ�����ֳ�ͼԪ
            if( !pRTC->updateData( false ) )
            {
                // ����,���˳�
                delete pRTC;
                clean();
                break;
            }
            pTm->pRTCModel = pRTC;
        }
        mc.append( pTm );
    }
}

void AirFlowCaculate::clean()
{
    if( mc.isEmpty() ) return;
    int len = mc.length();
    for( int i = 0; i < len; i++ )
    {
        delete mc[i];
    }
    mc.removeAll();
}
