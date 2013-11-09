#include "StdAfx.h"
#include "Eval_109.h"

#include "../MineGE/DrawHelper.h"
#include "../MineGE/DataListHelper.h"

#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"

#include "../VentNetworkAlgo/VNAlgoHelper.h"

/* 全局函数(实现在Tool.cpp中) */
extern bool HasGate( const AcDbObjectId& objId );

// 如果字符串为空白，则改成NULL
static void EmptyString2NULL( CString& name )
{
    name.Trim();
    if( name.GetLength() == 0 )
    {
        name = "NULL";
    }
}

/**********************************************************/
/************ EvalDataExtractor_109 ***********/
/**********************************************************/
EvalDataExtractor_109::EvalDataExtractor_109()
{
    acutPrintf( _T( "\n联络巷评价" ) );
    init();
    acutPrintf( _T( "\n找到可评价数据的个数:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_109::~EvalDataExtractor_109()
{
    clean();
}

static bool GetEdgeName( const AcDbObjectId& objId, CString& name )
{
    bool ret = true;
    ILTDataLink tn;
    tn.setDataSource( objId );
    if( !tn.updateData( false ) )
    {
        ret = false;
    }
    else
    {
        name = tn.name;
    }
    return ret;
}

// 使用datalink比较的麻烦，也可以使用其它的方法
// 例如DataHelper中的get/set方法
void EvalDataExtractor_109::init()
{
    acutPrintf( _T( "\n109 -- 查找联络巷" ) );

    // 查找联络巷
    AcDbObjectIdArray objIds;
    ILTHelper ilt;
    if( !ilt.doILT( objIds ) ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObjectId objId = objIds[i];

        // 查找联络巷的入边和出边巷道
        AcDbObjectIdArray s1, s2;
        if( !ilt.findInOutEdges( objId, s1, s2 ) ) continue;

        ILTDataLink* pTN = new ILTDataLink();
        pTN->setDataSource( objId );
        if( !pTN->updateData( false ) )
        {
            delete pTN;
            break;
        }

        if( !s1.isEmpty() )
        {
            GetEdgeName( s1[0], pTN->inEdgeName ); // 获取入边巷道名称
        }
        if( !s2.isEmpty() )
        {
            GetEdgeName( s2[0], pTN->outEdgeName ); // 获取出边巷道名称
        }

        // 判断联络巷上是否有构筑物
        pTN->hasWall = HasGate( objId );

        ds.push_back( pTN );
    }
    acutPrintf( _T( "\n109 -- 查找完毕" ) );
}

void EvalDataExtractor_109::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete *itr;
    }
    ds.clear();
}

bool EvalDataExtractor_109::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_109::doExtract( EvalData* pEvalData )
{
    ILTDataLink* pTN = *itr;
    EvalData_109* pVData = ( EvalData_109* )pEvalData;

    pVData->name = pTN->name;
    pVData->inEdgeName = pTN->inEdgeName;
    pVData->outEdgeName = pTN->outEdgeName;
    pVData->hasWall = pTN->hasWall;

    itr++;
}

/**********************************************************/
/********************* EvalData_109 *******************/
/**********************************************************/
EvalData_109::EvalData_109()
{

}

void EvalData_109::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_STRING_FIELD( 主进风巷 )
    DEFINE_STRING_FIELD( 主回风巷 )
    DEFINE_STRING_FIELD( 联络巷 )
    DEFINE_STRING_FIELD( 构筑物 )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}

void EvalData_109::writeToTableImpl( DataWriter* pDataWriter )
{
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    EmptyString2NULL( inEdgeName );
    WRITE_STRING_DATA( 主进风巷, inEdgeName )
    EmptyString2NULL( outEdgeName );
    WRITE_STRING_DATA( 主回风巷, outEdgeName )
    EmptyString2NULL( name );
    WRITE_STRING_DATA( 联络巷, name )
    if( hasWall )
    {
        WRITE_STRING_DATA( 构筑物, _T( "有" ) )
    }
    else
    {
        WRITE_STRING_DATA( 构筑物, _T( "无" ) )
    }
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/********************** Eval_109 **********************/
/**********************************************************/

bool Eval_109::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_109::eval_once( EvalData* pEvalData )
{
    EvalData_109* pVData = ( EvalData_109* )pEvalData;
    return ( pVData->hasWall );
}

/**********************************************************/
/*************** EvalFactory_109 **************/
/**********************************************************/
Eval* EvalFactory_109::createEvalObject()
{
    return new Eval_109();
}

EvalData* EvalFactory_109::createEvalDataObject()
{
    return new EvalData_109();
}

EvalDataExtractor* EvalFactory_109::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_109();
}