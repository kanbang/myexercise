#include "StdAfx.h"
#include "Eval_131.h"

#include "../MineGE/DrawHelper.h"
#include "../MineGE/DataListHelper.h"
#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"

#include "../VentNetworkAlgo/VNAlgoHelper.h"

static void AppendPathName( const AcDbObjectIdArray& objIds, AcStringArray& p )
{
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TunnelNameDataLink tn;
        tn.setDataSource( objIds[i] );
        tn.updateData( false ); // 这里就不检查错误了
        //acutPrintf(_T("\n\t增加分支:%s"), tn.name);
        p.append( tn.name );
    }
}

// 如果字符串为空白，则改成NULL
static void EmptyString2NULL( CString& name )
{
    name.Trim();
    if( name.GetLength() == 0 )
    {
        name = "NULL";
    }
}

// 将路径字符串用"->"连接起来
static CString MakePathString( AcStringArray& p )
{
    CString str;
    int len = p.length();
    for( int i = 0; i < len; i++ )
    {
        CString name = p[i].kACharPtr();
        EmptyString2NULL( name );
        str.AppendFormat( _T( "%s->" ), name );
    }
    return str;
}

/**********************************************************/
/************ EvalDataExtractor_131 ***********/
/**********************************************************/
EvalDataExtractor_131::EvalDataExtractor_131()
{
    acutPrintf( _T( "\n独立路线评价" ) );
    init();
    acutPrintf( _T( "\n找到可评价数据的个数:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_131::~EvalDataExtractor_131()
{
    clean();
}

void EvalDataExtractor_131::init()
{
    acutPrintf( _T( "\n131 -- 查找充电室独立通风路线" ) );

    // 查找所有的爆炸材料库
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "ChargeRoom" ), objIds );

    FIPHelper fip;
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        //acutPrintf(_T("\n第%d个用风地点开始fip..."), i);
        AcDbObjectIdArray p1, p2;
        if( !fip.doFIP( objIds[i], p1, p2 ) ) continue;

        //acutPrintf(_T("\n第%d个用风地点成功结束fip..."), i);

        FipDataLink* pFD = new FipDataLink();
        pFD->setDataSource( objIds[i] );
        if( !pFD->updateData( false ) )
        {
            //acutPrintf(_T("\n131更新数据错误"));
            delete pFD;
            clean();
            return;
        }
        AppendPathName( p1, pFD->p1 );
        AppendPathName( p2, pFD->p2 );
        pFD->includeAir = ( fip.isIncludeAirEdge( p1 ) || fip.isIncludeAirEdge( p2 ) );

        ds.push_back( pFD ); // 添加到集合中
    }
    acutPrintf( _T( "\n131 -- 查找完毕" ) );
}

void EvalDataExtractor_131::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete *itr;
    }
    ds.clear();
}

bool EvalDataExtractor_131::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_131::doExtract( EvalData* pEvalData )
{
    FipDataLink* pFD = *itr;
    EvalData_131* pVData = ( EvalData_131* )pEvalData;

    pVData->name = pFD->name;
    pVData->p1.append( pFD->p1 );
    pVData->p2.append( pFD->p2 );
    pVData->includeAir = pFD->includeAir;

    itr++;
}

/**********************************************************/
/********************* EvalData_131 *******************/
/**********************************************************/
EvalData_131::EvalData_131()
{

}

void EvalData_131::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_COMMENT_FIELD( 进风通路 )
    DEFINE_STRING_FIELD( 独立用风地点 )
    DEFINE_COMMENT_FIELD( 回风通路 )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}

void EvalData_131::writeToTableImpl( DataWriter* pDataWriter )
{
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    WRITE_STRING_DATA( 进风通路, MakePathString( p1 ) )
    EmptyString2NULL( name );
    WRITE_STRING_DATA( 独立用风地点, name )
    WRITE_STRING_DATA( 回风通路, MakePathString( p2 ) )
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/********************** Eval_131 **********************/
/**********************************************************/

bool Eval_131::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_131::eval_once( EvalData* pEvalData )
{
    EvalData_131* pVData = ( EvalData_131* )pEvalData;
    return !( pVData->includeAir );
}

/**********************************************************/
/*************** EvalFactory_131 **************/
/**********************************************************/
Eval* EvalFactory_131::createEvalObject()
{
    return new Eval_131();
}

EvalData* EvalFactory_131::createEvalDataObject()
{
    return new EvalData_131();
}

EvalDataExtractor* EvalFactory_131::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_131();
}