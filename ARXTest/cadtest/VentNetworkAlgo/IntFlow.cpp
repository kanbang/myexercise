#include "StdAfx.h"
#include "IntFlow.h"

#include "../ArxHelper/ArxUtilHelper.h"
#include "../ArxHelper/ArxEntityHelper.h"

#include "../GraphTool/Tool.h"

/* 全局函数(实现在DisplayIntValue.cpp中) */
extern void DisplayIntValue( const AcDbObjectIdArray& objIds, const AcDbIntArray& ivs );

static void DisplayIntValue( Digraph& dg, ArcIntFlowMap& datas )
{
    AcDbObjectIdArray objIds;
    AcDbIntArray ivs;
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        objIds.append( datas[e]->objId );
        ivs.append( datas[e]->iv );
    }

    DisplayIntValue( objIds, ivs );
}

static void ClearGraph( Digraph& dg, ArcIntFlowMap& datas )
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        delete datas[e];
        datas[e] = 0; // 防止野指针
    }
    dg.clear();       // 删除图
}

// 设置独立用风分支
// 独立用风地点：采掘工作面、采区变电所
static void SetEdgeType( const AcDbObjectId& objId, ArcIntFlowData* pEdgeData )
{
    if( objId.isNull() )
    {
        pEdgeData->et = ET_VIRTUAL;
    }
    if( ArxUtilHelper::IsEqualType( _T( "WorkSurface" ), objId ) )
    {
        pEdgeData->et = ET_USEAIR;  // 工作面
    }
    else if( ArxUtilHelper::IsEqualType( _T( "StorageGE" ), objId ) )
    {
        if( IsPowerSubstation( objId ) )
        {
            pEdgeData->et = ET_USEAIR;  // 变电所
        }
    }
    else if( ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId ) )
    {
        pEdgeData->et = ET_USEAIR;  // 掘进工作面
    }
    /*else if(IsNeedAir(objId))
    {
    pEdgeData->et = ET_USEAIR;
    }*/
}

static void InitEdgeDatas( Digraph& dg, ObjectIdMap& om, ArcIntFlowMap& datas )
{
    // 关联分支属性
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        ArcIntFlowData* pEdgeData = new ArcIntFlowData();
        pEdgeData->objId = om[e];
        SetEdgeType( pEdgeData->objId, pEdgeData );
        datas[e] = pEdgeData; // 关联一个Digraph::Arc属性
    }
}

IntFlow::IntFlow( void ) : datas( dg ), ef( dg, true ), bInitNetwokState( true )
{
    // 初始化网络以及添加处理源汇
    if( !( initNetwok() ) )
    {
        ClearGraph( dg, datas );
        setInitNetwokState( false );
    }
    else
    {
        initAirEdges();   // 记录用风地点分支
    }
}

IntFlow::~IntFlow( void )
{
    if( bInitNetwokState )
    {
        ClearGraph( dg, datas );
    }
}

Digraph::Arc IntFlow::getEdgeById( const AcDbObjectId& objId )
{
    if( objId.isNull() ) return INVALID;

    Digraph::Arc ee = INVALID;
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( datas[e]->objId == objId )
        {
            ee = e;
            break;
        }
    }
    return ee;
}

AcDbObjectId IntFlow::getIdByEdge( Digraph::Arc e )
{
    if( e == INVALID ) return AcDbObjectId::kNull;
    // 应该判断分支是否包含在图中
    // 目前就假设e肯定是图的分支
    return datas[e]->objId;
}

bool IntFlow::initNetwok()
{
    // 构建网络
    ObjectIdMap om( dg );
    if( !BuildNetwork( dg, om ) ) return false;
    FilterBlockEdges( dg, om, ef );
    if( !AddVirtualSTNode( dg, om, sn, tn ) ) return false;

    // 关联分支属性数据
    InitEdgeDatas( dg, om, datas );

    return true;
}

bool IntFlow::isInitNetworkOk() const
{
    return bInitNetwokState;
}

void IntFlow::setInitNetwokState( bool bState )
{
    bInitNetwokState = bState;
}

void IntFlow::initAirEdges()
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( datas[e]->et == ET_USEAIR )
        {
            airEdges.append( e );
        }
    }
}

// 流入的总流量-流出的流量(排除分支e)
int IntFlow::caclEdgeOutFlow( Digraph::Arc e )
{
    Digraph::Node u = dg.source( e );

    // 入边总流量
    int iq = 0;
    for( Digraph::InArcIt ie( dg, u ); ie != INVALID; ++ie )
    {
        iq += ( datas[ie]->iv );
    }

    // 出边流量(排除分支e)
    int oq = 0;
    for( Digraph::OutArcIt oe( dg, u ); oe != INVALID; ++oe )
    {
        if( oe == e ) continue;
        oq += ( datas[oe]->iv );
    }

    return ( iq - oq );
}

// 流出的总流量-流入的流量(排除分支e)
int IntFlow::caclEdgeInFlow( Digraph::Arc e )
{
    Digraph::Node u = dg.target( e );

    // 出边总流量
    int oq = 0;
    for( Digraph::OutArcIt oe( dg, u ); oe != INVALID; ++oe )
    {
        oq += ( datas[oe]->iv );
    }

    // 入边流量(排除分支e)
    int iq = 0;
    for( Digraph::InArcIt ie( dg, u ); ie != INVALID; ++ie )
    {
        if( ie == e ) continue;
        iq += ( datas[ie]->iv );
    }

    return ( oq - iq );
}

void IntFlow::initEdgeFlow()
{
    acutPrintf( _T( "\n初始化..." ) );
    // 初始化所有分支iv=1
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        datas[e]->iv = 1;
        //acutPrintf(_T("\n分支:e%d\tiv=%d"), dg.id(e), datas[e]->iv);
    }
}

void IntFlow::doCacl()
{
    initEdgeFlow();

    // 最大流量(目前粗略估计为分支数)
    int maxQ = countArcs( dg );

    acutPrintf( _T( "\n开始计算..." ) );
    bool bQuit = false;
    int count = 0;
    while( !bQuit && count <= maxQ )
    {
        count++;
        bQuit = true;
        for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
        {
            int oq = caclEdgeOutFlow( e );
            int iq = caclEdgeInFlow( e );
            int mq = max( oq, iq );
            //acutPrintf(_T("\n分支:e%d   iq=%d   oq=%d\t最大风量:%d"), dg.id(e), iq, oq, mq);

            if( mq > datas[e]->iv )
            {
                bQuit = false;
                datas[e]->iv = mq;
            }
        }
        //acutPrintf(_T("\n===========================================\n"));
    }
    acutPrintf( _T( "\ncount=%d" ), count );

    // 测试用，显示ivalue
    DisplayIntValue( dg, datas );
}

