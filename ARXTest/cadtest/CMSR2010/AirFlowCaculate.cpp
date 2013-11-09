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
        acutPrintf( _T( "\nɾ���������������ݿ�..." ) );
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
        acutPrintf( _T( "\n%s ���з�������" ), pWs->name );
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
    // 1����ը���Ͽ��������
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
    // 2������ҷ�������
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
    // 3���������ҷ�������
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
            // ���ͻ�������
            dhc_afc.initLargeMEC_AFCStrategy( pMm->power, pMm->theta, pMm->delta_t );
        }
        else
        {
            // С�ͻ�������
            dhc_afc.initSmallMEC_AFCStrategy( pMm->power, 80 ); // ȡ80
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
        ds.append( pWsm );
    }
}

void AirFlowCaculate::init_TWSModel()
{
    // 1���������еľ��������ͼԪ
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
        ds.append( pTwsm );
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
        ds.append( pPm );
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
        ds.append( pCm );
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
        ds.append( pMm );
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
        if( !pTm->updateData( false ) ) // ��ȡ����ʧ��
        {
            // ����,���˳�
            delete pTm;
            clean();
            break;
        }

        int ft = ( pTm->ft % 6 ); // ��Χ:0-5

        // ����Ҫ������磬���޼��ߵ糵�ͽ��ֳ�����
        // �����з�������
        if( !pTm->needAir && ft != FT_ELECTRIC && ft != FT_DIESEL )
        {
            delete pTm;
            continue;
        }

        pTm->an = ( AFC_NUM )( AN_1 | AN_2 ); // Ĭ����Ҫ������˹����ͷ�������
        if( ft == FT_ELECTRIC ) pTm->an = ( AFC_NUM )( pTm->an | AN_3 );
        if( ft == FT_DIESEL ) pTm->an = ( AFC_NUM )( pTm->an | AN_4 );

        ds.append( pTm );
    }
}

// ����delete����Ϊds������Ҫ���ⲿ�����۳���ʹ��
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
