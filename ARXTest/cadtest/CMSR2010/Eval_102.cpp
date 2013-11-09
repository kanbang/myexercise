#include "StdAfx.h"
#include "Eval_102.h"

#include "../MineGE/DrawHelper.h"

/* 全局函数(实现在Tool.cpp中) */
extern int FindShafts( AcDbObjectIdArray& shafts );

#define MIN_TEMP 0
#define MAX_TEMP 50

/**********************************************************/
/************ EvalDataExtractor_102 ***********/
/**********************************************************/
EvalDataExtractor_102::EvalDataExtractor_102()
{
    acutPrintf( _T( "\n102--温度评价..." ) );
    init();
    acutPrintf( _T( "\n找到可评价数据的个数:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_102::~EvalDataExtractor_102()
{
    clean();
}

void EvalDataExtractor_102::init()
{
    // 暂时排除普通巷道
    // 如果评价所有的普通巷道的温度，可能导致word报告的输出速度非常慢
    // 通常情况下，普通巷道的温度大都能满足规程要求
    //init1();
    init2();
    init3();
    //init4();
}

// 查找进风井
static void FindInShafts( AcDbObjectIdArray& inShafts )
{
    // 查找全部井筒
    AcDbObjectIdArray shafts;
    int pos = FindShafts( shafts );

    for( int i = 0; i < pos; i++ )
    {
        inShafts.append( shafts[i] );
    }
}

void EvalDataExtractor_102::init1()
{
    acutPrintf( _T( "\n102 -- 查找所有非进风井巷道" ) );

    // 查找所有的进风井
    AcDbObjectIdArray inShafts;
    FindInShafts( inShafts );

    // 1、查找所有巷道
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "Tunnel" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2、关联
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TempDataLink* pStm = new TempDataLink();
        pStm->setDataSource( objIds[i] );
        if( !pStm->updateData( false ) )
        {
            // 清理,并退出
            delete pStm;
            clean();
            break;
        }
        if( inShafts.contains( objIds[i] ) ) continue; // 排除进风井

        pStm->type = TEMP_COMMON; // 普通巷道
        //acutPrintf(_T("\n%name:%s\t%.4f"), pStm->name, pStm->t);
        ds.push_back( pStm );
    }
    acutPrintf( _T( "\n102 -- 查找完毕" ) );
}

void EvalDataExtractor_102::init2()
{
    acutPrintf( _T( "\n102 -- 查找所有采掘工作面" ) );
    // 1、查找所有采掘工作面
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "WorkSurface" ), objIds );
    DrawHelper::FindMineGEs( _T( "TTunnel" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2、关联
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TempDataLink* pTm = new TempDataLink();
        pTm->setDataSource( objIds[i] );
        if( !pTm->updateData( false ) )
        {
            // 清理,并退出
            delete pTm;
            clean();
            break;
        }
        pTm->type = TEMP_TWS; // 采掘面
        //acutPrintf(_T("\n%name:%s\t%.4f"), pTm->name, pTm->t);
        ds.push_back( pTm );
    }
    acutPrintf( _T( "\n102 -- 查找完毕" ) );
}

void EvalDataExtractor_102::init3()
{
    acutPrintf( _T( "\n102 -- 查找所有机电设备硐室图元" ) );

    // 1、查找所有机电硐室图元
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "MachineRoom" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2、关联
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TempDataLink* pTm = new TempDataLink();
        pTm->setDataSource( objIds[i] );
        if( !pTm->updateData( false ) )
        {
            // 清理,并退出
            delete pTm;
            clean();
            break;
        }
        pTm->type = TEMP_MR; // 机电硐室
        acutPrintf( _T( "\n%name:%s\t%.4f" ), pTm->name, pTm->t );
        ds.push_back( pTm );
    }
    acutPrintf( _T( "\n102 -- 查找完毕" ) );
}

void EvalDataExtractor_102::init4()
{
    acutPrintf( _T( "\n102 -- 查找其它图元(火药库、充电室)" ) );

    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "PowderStorage" ), objIds ); // 火药库
    DrawHelper::FindMineGEs( _T( "ChargeRoom" ), objIds ); // 充电室
    if( objIds.isEmpty() ) return;

    // 2、关联
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TempDataLink* pTm = new TempDataLink();
        pTm->setDataSource( objIds[i] );
        if( !pTm->updateData( false ) )
        {
            // 清理,并退出
            delete pTm;
            clean();
            break;
        }
        pTm->type = TEMP_MR2;
        acutPrintf( _T( "\n%name:%s\t%.4f" ), pTm->name, pTm->t );
        ds.push_back( pTm );
    }
    acutPrintf( _T( "\n102 -- 查找完毕" ) );
}

void EvalDataExtractor_102::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete ( *itr );
    }
    ds.clear();
}

bool EvalDataExtractor_102::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_102::doExtract( EvalData* pEvalData )
{
    TempDataLink* pVM = *itr;
    EvalData_102* pVData = ( EvalData_102* )pEvalData;

    pVData->name = pVM->name;
    pVData->t = pVM->t;
    pVData->minT = MIN_TEMP;
    pVData->maxT = MAX_TEMP;
    pVData->type = pVM->type;

    switch( pVM->type )
    {
    case TEMP_COMMON:
    {
        initMinMaxTemperature1( pVData );
    }
    break;
    case TEMP_TWS:
    {
        initMinMaxTemperature2( pVData );
    }
    break;
    case TEMP_MR:
    {
        initMinMaxTemperature3( pVData );
    }
    break;

    case TEMP_MR2:
        initMinMaxTemperature4( pVData );
        break;
    }
    itr++;
}

void EvalDataExtractor_102::initMinMaxTemperature1( EvalData_102* pVData )
{
    pVData->minT = 2;
    pVData->maxT = MAX_TEMP; // 温度上限(50)
}

void EvalDataExtractor_102::initMinMaxTemperature2( EvalData_102* pVData )
{
    pVData->minT = 2;
    pVData->maxT = 26;
}

void EvalDataExtractor_102::initMinMaxTemperature3( EvalData_102* pVData )
{
    pVData->minT = 2;
    pVData->maxT = 30;
}

void EvalDataExtractor_102::initMinMaxTemperature4( EvalData_102* pVData )
{
    pVData->minT = 2;
    pVData->maxT = 26;
}

/**********************************************************/
/********************* EvalData_102 *******************/
/**********************************************************/
EvalData_102::EvalData_102() : minT( MIN_TEMP ), maxT( MAX_TEMP )
{

}

void EvalData_102::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_STRING_FIELD( 测点所在地名称 )
    DEFINE_REAL_FIELD( 干球温度 )
    DEFINE_STRING_FIELD( 最低温度 )
    DEFINE_STRING_FIELD( 最高温度 )
    DEFINE_STRING_FIELD( 控制措施 )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}
void EvalData_102::writeToTableImpl( DataWriter* pDataWriter )
{
    CString emptyStr = _T( "" );
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    WRITE_STRING_DATA( 测点所在地名称, name )
    WRITE_REAL_DATA( 干球温度, t )
    if( minT == MIN_TEMP )
    {
        WRITE_STRING_DATA( 最低温度, emptyStr );
    }
    else
    {
        CString value;
        value.Format( _T( "%d" ), ( int )minT );
        WRITE_STRING_DATA( 最低温度, value )
    }
    if( maxT == MAX_TEMP )
    {
        WRITE_STRING_DATA( 最高温度, emptyStr );
    }
    else
    {
        CString value;
        /*if(type == TEMP_TWS)
        {
        	value.Format(_T("采掘工作面--%d"),(int)maxT);
        }
        else if(type == TEMP_MR)
        {
        	value.Format(_T("机电硐室--%d"),(int)maxT);
        }
        else
        {*/
        value.Format( _T( "%d" ), ( int )maxT );
        //}
        WRITE_STRING_DATA( 最高温度, value )
    }
    WRITE_STRING_DATA( 控制措施, strCM )
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/********************** Eval_102 **********************/
/**********************************************************/

bool Eval_102::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_102::eval_once( EvalData* pEvalData )
{
    EvalData_102* pVData = ( EvalData_102* )pEvalData;
    double t = pVData->t;
    double minT = pVData->minT;
    double maxT = pVData->maxT;
    int cm = pVData->cm;
    CString& strCM = pVData->strCM;
    strCM = _T( "无" );

    bool ret = false;
    switch( pVData->type )
    {
    case TEMP_COMMON:
    {
        ret = ( t >= minT );
        strCM = _T( "" );
    }
    break;

    case TEMP_TWS:
    {
        if( t < minT )
        {
            ret = false;
        }
        else if( t <= 26 )
        {
            ret = true;
        }
        else if( t < 30 )
        {
            ret = false;
            if( cm == 1 )
            {
                ret = true;
                strCM = _T( "缩短工作时间，给予高温保健待遇" );
            }
            else if( cm == 2 )
            {
                ret = true;
                strCM = _T( "停止作业" );
            }
        }
        else
        {
            ret = false;
            if( cm == 2 )
            {
                ret = true;
                strCM = _T( "停止作业" );
            }
        }
    }
    break;

    case TEMP_MR:
    {
        if( t < minT )
        {
            ret = false;
        }
        else if( t <= 30 )
        {
            ret = true;
        }
        else if( t < 34 )
        {
            ret = false;
            if( cm == 1 )
            {
                ret = true;
                strCM = _T( "缩短工作时间，给予高温保健待遇" );
            }
            else if( cm == 2 )
            {
                ret = true;
                strCM = _T( "停止作业" );
            }
        }
        else
        {
            ret = false;
            if( cm == 2 )
            {
                ret = true;
                strCM = _T( "停止作业" );
            }
        }
    }
    break;

    case TEMP_MR2:
    {
        if( t < minT )
        {
            ret = false;
        }
        else if( t <= maxT )
        {
            ret = true;
        }
        else
        {
            ret = false;
            if( cm == 1 )
            {
                ret = true;
                strCM = _T( "缩短工作时间，给予高温保健待遇" );
            }
            else if( cm == 2 )
            {
                ret = true;
                strCM = _T( "停止作业" );
            }
        }
    }
    break;
    }
    return ret;
}

/**********************************************************/
/*************** EvalFactory_102 **************/
/**********************************************************/
Eval* EvalFactory_102::createEvalObject()
{
    return new Eval_102();
}

EvalData* EvalFactory_102::createEvalDataObject()
{
    return new EvalData_102();
}

EvalDataExtractor* EvalFactory_102::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_102();
}