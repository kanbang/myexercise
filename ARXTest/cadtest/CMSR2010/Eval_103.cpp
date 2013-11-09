#include "StdAfx.h"
#include "Eval_103.h"

#include "../MineGE/DataListHelper.h"
#include "../MineGE/DataHelper.h"

#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"

#include "AirFlowCaculate.h"

// 在EvalData_103::createTableImpl()方法中赋值
static CString EvalDataBaseDirPath = _T( "" ); // 评价结果数据库的所在文件夹路径

// 将数据从AcArray复制到std::deque
static void CopyFromAcArray( AcArray<AFCDataLink*>& ts, std::deque<AFCDataLink*>& ds )
{
    int len = ts.length();
    for( int i = 0; i < len; i++ )
    {
        ds.push_back( ts[i] );
    }
}

// 读取矿井的瓦斯等级数据
static int ReadGasLevel()
{
    AcDbObjectId objId;
    if( !SingleDataObjectHelper::GetObjectId( _T( "矿井信息" ), objId ) ) return GL_LOW;

    CString value;
    if( !DataHelper::GetPropertyData( objId, _T( "瓦斯等级" ), value ) ) return GL_LOW;

    // 这里假定数据格式总是正确的({0,1,2}--GAS_LEVEL)
    return _ttoi( value ) % 3;
}

/**********************************************************/
/************ EvalDataExtractor_103 ***********/
/**********************************************************/
EvalDataExtractor_103::EvalDataExtractor_103()
{
    init();
    acutPrintf( _T( "\n找到可评价数据的个数:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_103::~EvalDataExtractor_103()
{
    clean();
}

void EvalDataExtractor_103::init()
{
    acutPrintf( _T( "\n风量计算评价" ) );
    acutPrintf( _T( "\n103 -- 初始化" ) );

    // 读取矿井的瓦斯等级信息
    gas_level = ReadGasLevel();

    // 计算风量
    AcArray<AFCDataLink*> ts;
    AirFlowCaculate afc;
    afc.setMineGasState( !( gas_level == GL_LOW ) );

    acutPrintf( _T( "\n风量计算路径:%s" ), EvalDataBaseDirPath );

    CString path;
    path.Format( _T( "%s\\%s" ), EvalDataBaseDirPath, _T( "afc.mdb" ) );
    afc.doAFC( path, ts ); // 执行风量计算，并生成afc.mdb数据库

    // 得到所有的数据集合
    CopyFromAcArray( ts, ds );

    acutPrintf( _T( "\n103 -- 初始化完毕" ) );
}

void EvalDataExtractor_103::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete *itr;
    }
    ds.clear();
}

bool EvalDataExtractor_103::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_103::doExtract( EvalData* pEvalData )
{
    AFCDataLink* pAFC = *itr;
    EvalData_103* pVData = ( EvalData_103* )pEvalData;

    pVData->name = pAFC->name;
    pVData->q = pAFC->q;
    pVData->maxQ = pAFC->maxQ;

    itr++;
}

/**********************************************************/
/********************* EvalData_103 *******************/
/**********************************************************/
EvalData_103::EvalData_103()
{

}

void EvalData_103::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_STRING_FIELD( 用风地点名称 )
    DEFINE_REAL_FIELD( 需风量 )
    DEFINE_REAL_FIELD( 实际风量 )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );

    // 获取数据库文件夹路径
    EvalDataBaseDirPath = pTableCreator->getDBDirPath();
}

void EvalData_103::writeToTableImpl( DataWriter* pDataWriter )
{
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    WRITE_STRING_DATA( 用风地点名称, name )
    WRITE_REAL_DATA( 需风量, maxQ )
    WRITE_REAL_DATA( 实际风量, q )
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/********************** Eval_103 **********************/
/**********************************************************/

bool Eval_103::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_103::eval_once( EvalData* pEvalData )
{
    EvalData_103* pVData = ( EvalData_103* )pEvalData;
    bool ret = true;
    if( ( pVData->maxQ < 0 ) || ( pVData->q < 0 ) )
    {
        ret = false;
    }
    else
    {
        ret = ( pVData->q >= pVData->maxQ );
    }
    return ret;
}

/**********************************************************/
/*************** EvalFactory_103 **************/
/**********************************************************/
Eval* EvalFactory_103::createEvalObject()
{
    return new Eval_103();
}

EvalData* EvalFactory_103::createEvalDataObject()
{
    return new EvalData_103();
}

EvalDataExtractor* EvalFactory_103::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_103();
}