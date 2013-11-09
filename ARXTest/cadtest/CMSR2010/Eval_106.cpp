#include "StdAfx.h"
#include "Eval_106.h"

#include "../MineGE/DrawHelper.h"
#include "../MineGE/DataListHelper.h"
#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"

/**********************************************************/
/************ EvalDataExtractor_106 ***********/
/**********************************************************/
EvalDataExtractor_106::EvalDataExtractor_106()
{
    acutPrintf( _T( "\n安全仪表评价" ) );
    init();
    acutPrintf( _T( "\n找到可评价数据的个数:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_106::~EvalDataExtractor_106()
{
    clean();
}

void EvalDataExtractor_106::init()
{
    acutPrintf( _T( "\n106 -- 查找安全仪表" ) );

    AcStringArray names;
    DataObjectListHelper::GetAllNames( names );
    if( names.isEmpty() || !names.contains( _T( "安全检测仪表" ) ) ) return;

    // 获取所有安全仪表
    AcStringArray values;
    DataObjectListHelper::GetObjectNameList( _T( "安全检测仪表" ), values );
    if( values.isEmpty() ) return;

    int len = values.length();
    for( int i = 0; i < len; i++ )
    {
        InstrumentDataLink* pID = new InstrumentDataLink();
        AcDbObjectId objId;
        DataObjectListHelper::GetObjectId( _T( "安全检测仪表" ), values[i].kACharPtr(), objId );
        pID->setDataSource( objId );
        if( !pID->updateData( false ) )
        {
            delete pID;
            clean();
            break;
        }
        ds.push_back( pID );
    }
    acutPrintf( _T( "\n106 -- 查找完毕" ) );
}

void EvalDataExtractor_106::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete *itr;
    }
    ds.clear();
}

bool EvalDataExtractor_106::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_106::doExtract( EvalData* pEvalData )
{
    InstrumentDataLink* pID = *itr;
    EvalData_106* pVData = ( EvalData_106* )pEvalData;

    pVData->name = pID->name;
    pVData->mn = pID->mn;
    pVData->amount = pID->amount;
    pVData->qi = pID->qi;

    itr++;
}

/**********************************************************/
/********************* EvalData_106 *******************/
/**********************************************************/
EvalData_106::EvalData_106()
{

}

void EvalData_106::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_STRING_FIELD( 仪表名称 )
    DEFINE_STRING_FIELD( 仪表型号 )
    DEFINE_INT_FIELD( 仪表数量 )
    DEFINE_STRING_FIELD( 计量检验结果 )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}

void EvalData_106::writeToTableImpl( DataWriter* pDataWriter )
{
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    WRITE_STRING_DATA( 仪表名称, name )
    WRITE_STRING_DATA( 仪表型号, mn )
    WRITE_INT_DATA( 仪表数量, amount )
    if( amount > 0 )
    {
        WRITE_STRING_DATA( 计量检验结果, ( qi ? _T( "合格" ) : _T( "不合格" ) ) )
    }
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/********************** Eval_106 **********************/
/**********************************************************/

bool Eval_106::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_106::eval_once( EvalData* pEvalData )
{
    EvalData_106* pVData = ( EvalData_106* )pEvalData;
    bool ret = ( pVData->amount == 0 );
    if( !ret )
    {
        ret = pVData->qi;
    }
    return ret;
}

/**********************************************************/
/*************** EvalFactory_106 **************/
/**********************************************************/
Eval* EvalFactory_106::createEvalObject()
{
    return new Eval_106();
}

EvalData* EvalFactory_106::createEvalDataObject()
{
    return new EvalData_106();
}

EvalDataExtractor* EvalFactory_106::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_106();
}