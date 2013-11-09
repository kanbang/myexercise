#include "StdAfx.h"
#include "Eval_105.h"

#include "../MineGE/DrawHelper.h"
#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"

/* ȫ�ֺ���(ʵ����Tool.cpp��) */
extern void FindUseAirPlaces( AcDbObjectIdArray& objIds, int& p1, int& p2, int& p3 );

/**********************************************************/
/************ EvalDataExtractor_105 ***********/
/**********************************************************/
EvalDataExtractor_105::EvalDataExtractor_105()
{
    acutPrintf( _T( "\n���վ����" ) );
    init();
    acutPrintf( _T( "\n�ҵ����������ݵĸ���:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_105::~EvalDataExtractor_105()
{
    clean();
}

// ������Ҫ���ĵص�
static void FindUseAirPlaces( AcDbObjectIdArray& objIds )
{
    int p1, p2, p3;
    FindUseAirPlaces( objIds, p1, p2, p3 );
}

void EvalDataExtractor_105::init()
{
    acutPrintf( _T( "\n105 -- ���Ҳ��վ" ) );

    // ������Ҫ���ĵص�
    AcDbObjectIdArray objIds;
    FindUseAirPlaces( objIds );

    // ��ȡ����
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        StationTunnelDataLink* pST = new StationTunnelDataLink();
        pST->setDataSource( objIds[i] );
        if( !pST->updateData( false ) )
        {
            delete pST;
            // ��������ʧ��
            clean();
            break;
        }
        // �ж�����Ƿ��в��վ�Լ����վ�Ƿ񰴹涨����
        AcDbObjectIdArray stations;
        DrawHelper::GetTagGEById2( objIds[i], _T( "WindStation" ), stations );
        if( stations.isEmpty() )
        {
            pST->hasWindStation = false;
        }
        else
        {
            StationDataLink sd;
            sd.setDataSource( stations[0] ); // ����ֻ��һ�����վ������ж����ֻʹ�õ�1��
            if( !sd.updateData( false ) )
            {
                delete pST;
                clean();
                return; // ֱ���˳�
            }
            pST->hasWindStation = true;
            pST->hasDone = sd.hasDone;
        }
        ds.push_back( pST );
    }
    acutPrintf( _T( "\n105 -- �������" ) );
}

void EvalDataExtractor_105::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete *itr;
    }
    ds.clear();
}

bool EvalDataExtractor_105::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_105::doExtract( EvalData* pEvalData )
{
    StationTunnelDataLink* pST = *itr;
    EvalData_105* pVData = ( EvalData_105* )pEvalData;

    pVData->name = pST->name;
    pVData->hasWindStation = pST->hasWindStation;
    pVData->hasDone = pST->hasDone;

    itr++;
}

/**********************************************************/
/********************* EvalData_105 *******************/
/**********************************************************/
EvalData_105::EvalData_105()
{

}

void EvalData_105::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_STRING_FIELD( Ӧ���ص� )
    DEFINE_STRING_FIELD( ���޲��վ )
    DEFINE_STRING_FIELD( �Ƿ񰴹涨��� )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}

void EvalData_105::writeToTableImpl( DataWriter* pDataWriter )
{
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    WRITE_STRING_DATA( Ӧ���ص�, name )
    if( hasWindStation )
    {
        WRITE_STRING_DATA( ���޲��վ, _T( "��" ) )
        if( hasDone )
        {
            WRITE_STRING_DATA( �Ƿ񰴹涨���, _T( "��" ) )
        }
        else
        {
            WRITE_STRING_DATA( �Ƿ񰴹涨���, _T( "��" ) )
        }
    }
    else
    {
        WRITE_STRING_DATA( ���޲��վ, _T( "��" ) )
        WRITE_STRING_DATA( �Ƿ񰴹涨���, _T( "-" ) )
    }
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/********************** Eval_105 **********************/
/**********************************************************/

bool Eval_105::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_105::eval_once( EvalData* pEvalData )
{
    EvalData_105* pVData = ( EvalData_105* )pEvalData;
    return ( pVData->hasWindStation && pVData->hasDone );
}

/**********************************************************/
/*************** EvalFactory_105 **************/
/**********************************************************/
Eval* EvalFactory_105::createEvalObject()
{
    return new Eval_105();
}

EvalData* EvalFactory_105::createEvalDataObject()
{
    return new EvalData_105();
}

EvalDataExtractor* EvalFactory_105::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_105();
}