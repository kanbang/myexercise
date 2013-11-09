#include "StdAfx.h"
#include "Eval_101.h"

#include "../MineGE/DrawHelper.h"

#define MIN_VELOCITY 0
#define MAX_VELOCITY 100

/* 全局函数(实现在Tool.cpp中) */
extern int FindShafts( AcDbObjectIdArray& shafts );

/**********************************************************/
/************ EvalDataExtractor_101 ***********/
/**********************************************************/
EvalDataExtractor_101::EvalDataExtractor_101()
{
    acutPrintf( _T( "\n风速评价..." ) );
    init();
    acutPrintf( _T( "\n找到可评价数据的个数:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_101::~EvalDataExtractor_101()
{
    clean();
}

void EvalDataExtractor_101::init()
{
    init1();
    init2();
    init3();
    //init4();
}

void EvalDataExtractor_101::init1()
{
    acutPrintf( _T( "\101 -- 查找所有回采工作面图元" ) );
    // 1、查找所有回采工作面图元
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "WorkSurface" ), objIds );

    // 2、关联
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        WSVelocityDataLink* pWvm = new WSVelocityDataLink();
        pWvm->setDataSource( objIds[i] );
        if( !pWvm->updateData( false ) )
        {
            // 清理,并退出
            delete pWvm;
            clean();
            break;
        }
        //acutPrintf(_T("\n%name:%s\t%.4f\t%.4f\t%d\t%d"), pWvm->name, pWvm->v, pWvm->q_ch4, pWvm->tm, pWvm->cm);
        ds.push_back( pWvm );
    }
    acutPrintf( _T( "\n101 -- 查找完毕" ) );
}

void EvalDataExtractor_101::init2()
{
    acutPrintf( _T( "\101 -- 查找所有掘进工作面图元" ) );

    // 1、查找所有掘进工作面图元
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "TTunnel" ), objIds );

    // 2、关联
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TWSVelocityDataLink* pTWvm = new TWSVelocityDataLink();
        pTWvm->setDataSource( objIds[i] );
        if( !pTWvm->updateData( false ) )
        {
            // 清理,并退出
            delete pTWvm;
            clean();
            break;
        }
        //acutPrintf(_T("\n%name:%s\t%.4f\t%.4f\t%d"), pTWvm->name, pTWvm->v, pTWvm->q_ch4, pTWvm->tm);
        ds.push_back( pTWvm );
    }
    acutPrintf( _T( "\n101 -- 查找完毕" ) );
}

void EvalDataExtractor_101::init3()
{
    acutPrintf( _T( "\101 -- 查找其它巷道图元" ) );

    // 1、查找进风井和回风井
    AcDbObjectIdArray shafts;
    int pos = FindShafts( shafts );

    // 1、查找所有巷道图元
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "Tunnel" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2、关联
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TunnelVelocityDataLink* pTvm = new TunnelVelocityDataLink();
        pTvm->setDataSource( objIds[i] );
        if( !pTvm->updateData( false ) )
        {
            // 清理,并退出
            delete pTvm;
            clean();
            break;
        }
        pTvm->isShaft = shafts.contains( objIds[i] ); // 是否井筒

        // 如果把所有普通巷道都进行评价，可能导致word报告输出速度非常慢
        if( !( pTvm->isShaft ) && pTvm->ft == 0 ) continue; // 排除普通巷道

        //acutPrintf(_T("\n%name:%s\t%.4f\t%.4f"), pTvm->name, pTvm->v, pTvm->q_ch4);

        if( pTvm->isShaft )
        {
            ds.push_front( pTvm ); // 如果是井筒，就放在队列的前面
        }
        else
        {
            ds.push_back( pTvm );
        }
    }
    acutPrintf( _T( "\n101 -- 查找完毕" ) );
}

void EvalDataExtractor_101::init4()
{
    acutPrintf( _T( "\101 -- 查找风桥图元" ) );

    // 1、查找所有风桥图元
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "WindBridge" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2、关联
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        BridgeVelocityDataLink* pBv = new BridgeVelocityDataLink();
        pBv->setDataSource( objIds[i] );
        if( !pBv->updateData( false ) )
        {
            // 清理,并退出
            delete pBv;
            clean();
            break;
        }
        acutPrintf( _T( "\n%name:%s\t%.4f\t%.4f" ), pBv->name, pBv->v, pBv->q_ch4 );
        ds.push_back( pBv );
    }
    acutPrintf( _T( "\n101 -- 查找完毕" ) );
}

void EvalDataExtractor_101::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete ( *itr );
    }
    ds.clear();
}

bool EvalDataExtractor_101::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_101::doExtract( EvalData* pEvalData )
{
    VelocityDataLink* pVM = *itr;
    EvalData_101* pVData = ( EvalData_101* )pEvalData;

    pVData->name = pVM->name;
    pVData->q_ch4 = pVM->q_ch4;
    pVData->v = pVM->v;
    pVData->minV = 0;
    pVData->maxV = 0;

    switch( pVM->type )
    {
    case VD_TUNNEL:
    {
        TunnelVelocityDataLink* pTVM = ( TunnelVelocityDataLink* )pVM;
        pVData->ft = pTVM->ft;
        pVData->sp = pTVM->sp;
        pVData->isShaft = pTVM->isShaft;
        initMinMaxVelocity1( pVData );
    }
    break;

    case VD_WS:
    {
        WSVelocityDataLink* pWVM = ( WSVelocityDataLink* )pVM;
        pVData->tm = pWVM->tm;
        pVData->cm = pWVM->cm;
        initMinMaxVelocity2( pVData );
    }
    break;

    case VD_TWS:
    {
        TWSVelocityDataLink* pTWVM = ( TWSVelocityDataLink* )pVM;
        pVData->tm = pTWVM->tm;
        initMinMaxVelocity3( pVData );
    }
    break;
    case VD_BRIDGE:
    {
        initMinMaxVelocity4( pVData );
    }
    break;
    }

    itr++;
}

void EvalDataExtractor_101::initShaftVelocity( EvalData_101* pVData )
{
    pVData->minV = MIN_VELOCITY;
    int sp = pVData->sp;
    if( sp <= 0 )
    {
        pVData->typeName = _T( "错误的井筒属性" );
        pVData->typeName = _T( "无提升设备的风井" );
        pVData->maxV = 15;
    }
    else if( ( sp & SEP_LIFT_COAL ) != 0 ) // 提升煤炭
    {
        // 在第110条有提及
        if( ( sp & SEP_SKIP_HOIST ) != 0 ) // 箕斗提升
        {
            pVData->typeName = _T( "箕斗提升井筒" );
            pVData->maxV = 6;
        }
        else
        {
            pVData->typeName = _T( "装有带式输送机的井筒" );
            pVData->maxV = 4;
        }
    }
    else if( ( sp & SEP_HAS_GLADER_OR_REPAIR ) != 0 ) // 修理中的井筒
    {
        pVData->typeName = _T( "设有梯子间或修理中的井筒" );
        pVData->maxV = 8;
    }
    else if( ( sp & SEP_NO_LIFT_USE ) != 0 ) // 无提升设备的风井
    {
        pVData->typeName = _T( "无提升设备的风井" );
        pVData->maxV = 15;
    }
    else if( ( sp & SEP_LIFT_MATERIAL_OR_STUFF ) != 0 ) // 升降人员
    {
        if( ( sp & SEP_LIFT_MATERIAL ) != 0 )
        {
            if( ( sp & SEP_LIFT_STUFF ) != 0 )
            {
                pVData->typeName = _T( "专为升降物料的井筒" );
                pVData->maxV = 12;
            }
            else
            {
                pVData->typeName = _T( "升降人员和物料的井筒" );
                pVData->maxV = 8;
            }
        }
    }
    else
    {
        pVData->typeName = _T( "错误的井筒属性" );
        pVData->maxV = MAX_VELOCITY;
    }
}

void EvalDataExtractor_101::initMinMaxVelocity1( EvalData_101* pVData )
{
    if( pVData->isShaft )
    {
        initShaftVelocity( pVData );
    }
    else
    {
        switch( pVData->ft )
        {
        case FT_SIMPLE:
        {
            pVData->typeName = _T( "其它通风人行巷道" );
            pVData->minV = 0.15;
            pVData->maxV = MAX_VELOCITY;
        }
        break;

        case FT_MAIN:
        {
            pVData->typeName = _T( "主要进、回风巷" );
            pVData->minV = MIN_VELOCITY;
            pVData->maxV = 8;
        }
        break;

        case FT_SUBMAIN:
        {
            pVData->typeName = _T( "采区进、回风巷" );
            pVData->minV = 0.25;
            pVData->maxV = 6;
        }
        break;

        case FT_TRANSPORT:
        {
            pVData->typeName = _T( "运输机巷" );
            pVData->minV = 0.25;
            pVData->maxV = 6;
        }
        break;

        case FT_ELECTRIC:
        {
            pVData->maxV = 8;
            if( pVData->q_ch4 < 0.001 ) // 近似等于0
            {
                pVData->typeName = _T( "无瓦斯涌出的架线电车机车巷道" );
                pVData->minV = 0.5;
            }
            else
            {
                pVData->typeName = _T( "有瓦斯涌出的架线电车机车巷道" );
                pVData->minV = 1.0;
            }
        }
        break;
        // 没有考虑数据的错误情况???
        }
    }
}

void EvalDataExtractor_101::initMinMaxVelocity2( EvalData_101* pVData )
{
    pVData->minV = 0.25;
    if( pVData->cm == CMM_COMPOSITIVE ) // 综采
    {
        pVData->typeName = _T( "综合机械化采煤工作面" );
        pVData->maxV = 5;
    }
    else
    {
        pVData->typeName = _T( "普通采煤工作面" );
        pVData->maxV = 4;
    }
}

void EvalDataExtractor_101::initMinMaxVelocity3( EvalData_101* pVData )
{
    pVData->maxV = 4;
    if( pVData->cm == MT_ROCK )
    {
        pVData->typeName = _T( "掘进中的岩巷" );
        pVData->minV = 0.15;
    }
    else
    {
        pVData->typeName = _T( "掘进中的煤巷和半煤岩巷" );
        pVData->minV = 0.25;
    }
}

void EvalDataExtractor_101::initMinMaxVelocity4( EvalData_101* pVData )
{
    pVData->typeName = _T( "风桥" );
    pVData->maxV = 10;
}

/**********************************************************/
/********************* EvalData_101 *******************/
/**********************************************************/
EvalData_101::EvalData_101() : minV( MIN_VELOCITY ), maxV( MAX_VELOCITY )
{

}

void EvalData_101::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_STRING_FIELD( 井巷名称 )
    DEFINE_STRING_FIELD( 井巷类型 )
    DEFINE_REAL_FIELD( 风速 )
    DEFINE_STRING_FIELD( 最低风速 )
    DEFINE_STRING_FIELD( 最高风速 )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}

void EvalData_101::writeToTableImpl( DataWriter* pDataWriter )
{
    CString emptyStr = _T( "-" );
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    WRITE_STRING_DATA( 井巷名称, name )
    WRITE_STRING_DATA( 井巷类型, typeName )
    WRITE_REAL_DATA( 风速, v )
    if( minV == MIN_VELOCITY )
    {
        WRITE_STRING_DATA( 最低风速, emptyStr )
    }
    else
    {
        CString value;
        value.Format( _T( "%.2f" ), minV );
        WRITE_STRING_DATA( 最低风速, value )
    }
    if( maxV == MAX_VELOCITY )
    {
        WRITE_STRING_DATA( 最高风速, emptyStr )
    }
    else
    {
        CString value;
        value.Format( _T( "%.2f" ), maxV );
        WRITE_STRING_DATA( 最高风速, value )
    }
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/********************** Eval_101 **********************/
/**********************************************************/

bool Eval_101::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_101::eval_once( EvalData* pEvalData )
{
    EvalData_101* pVData = ( EvalData_101* )pEvalData;
    return eval_v( pVData->v, pVData->minV, pVData->maxV );
}

bool Eval_101::eval_v( double v, double minV, double maxV )
{
    // ???浮点数的比较可能存在问题???
    return ( ( v >= minV ) && ( v <= maxV ) ); // 合理的风速
}

/**********************************************************/
/*************** EvalFactory_101 **************/
/**********************************************************/
Eval* EvalFactory_101::createEvalObject()
{
    return new Eval_101();
}

EvalData* EvalFactory_101::createEvalDataObject()
{
    return new EvalData_101();
}

EvalDataExtractor* EvalFactory_101::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_101();
}