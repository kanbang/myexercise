#include "StdAfx.h"
#include "Eval_105.h"

#include "../MineGE/DrawHelper.h"
#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"

/* 全局函数(实现在Tool.cpp中) */
extern void FindUseAirPlaces( AcDbObjectIdArray& objIds, int& p1, int& p2, int& p3 );

/**********************************************************/
/************ EvalDataExtractor_105 ***********/
/**********************************************************/
EvalDataExtractor_105::EvalDataExtractor_105()
{
    acutPrintf( _T( "\n测风站评价" ) );
    init();
    acutPrintf( _T( "\n找到可评价数据的个数:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_105::~EvalDataExtractor_105()
{
    clean();
}

// 查找需要测风的地点
static void FindUseAirPlaces( AcDbObjectIdArray& objIds )
{
    int p1, p2, p3;
    FindUseAirPlaces( objIds, p1, p2, p3 );
}

void EvalDataExtractor_105::init()
{
    acutPrintf( _T( "\n105 -- 查找测风站" ) );

    // 查找需要测风的地点
    AcDbObjectIdArray objIds;
    FindUseAirPlaces( objIds );

    // 提取数据
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        StationTunnelDataLink* pST = new StationTunnelDataLink();
        pST->setDataSource( objIds[i] );
        if( !pST->updateData( false ) )
        {
            delete pST;
            // 更新数据失败
            clean();
            break;
        }
        // 判断巷道是否有测风站以及测风站是否按规定测试
        AcDbObjectIdArray stations;
        DrawHelper::GetTagGEById2( objIds[i], _T( "WindStation" ), stations );
        if( stations.isEmpty() )
        {
            pST->hasWindStation = false;
        }
        else
        {
            StationDataLink sd;
            sd.setDataSource( stations[0] ); // 假设只有一个测风站，如果有多个，只使用第1个
            if( !sd.updateData( false ) )
            {
                delete pST;
                clean();
                return; // 直接退出
            }
            pST->hasWindStation = true;
            pST->hasDone = sd.hasDone;
        }
        ds.push_back( pST );
    }
    acutPrintf( _T( "\n105 -- 查找完毕" ) );
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
    DEFINE_STRING_FIELD( 应测风地点 )
    DEFINE_STRING_FIELD( 有无测风站 )
    DEFINE_STRING_FIELD( 是否按规定测风 )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}

void EvalData_105::writeToTableImpl( DataWriter* pDataWriter )
{
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    WRITE_STRING_DATA( 应测风地点, name )
    if( hasWindStation )
    {
        WRITE_STRING_DATA( 有无测风站, _T( "有" ) )
        if( hasDone )
        {
            WRITE_STRING_DATA( 是否按规定测风, _T( "是" ) )
        }
        else
        {
            WRITE_STRING_DATA( 是否按规定测风, _T( "否" ) )
        }
    }
    else
    {
        WRITE_STRING_DATA( 有无测风站, _T( "无" ) )
        WRITE_STRING_DATA( 是否按规定测风, _T( "-" ) )
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