#include "StdAfx.h"
#include "DfsNetWork.h"

#include "../MineGE/ConstData.h"

#include "../ArxHelper/ArxUtilHelper.h"
#include "../ArxHelper/ArxEntityHelper.h"

#include <lemon/adaptors.h>
#include <lemon/dfs.h>

#include "../GraphTool/Tool.h"

static void ClearGraph( Digraph& dg, DFS_EdgeDataMap& datas )
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
static bool SetEdgeType( const AcDbObjectId& objId, EdgeData* pEdgeData )
{
    bool ret = true;
    if( objId.isNull() )
    {
        pEdgeData->et = ET_VIRTUAL;
    }
    else if( ArxUtilHelper::IsEqualType( _T( "WorkSurface" ), objId ) )
    {
        pEdgeData->et = ET_USEAIR;  // 工作面
    }
    else if( ArxUtilHelper::IsEqualType( _T( "StorageGE" ), objId ) )
    {
        if( !ArxUtilHelper::IsEqualType( _T( "MachineRoom" ), objId ) )
        {
            pEdgeData->et = ET_USEAIR;  // 火药库或充电室
        }
        else if( IsPowerSubstation( objId ) )
        {
            pEdgeData->et = ET_USEAIR;  // 机电硐室--变电所(用风地点)
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
    return ret;
}

static void InitEdgeDatas( Digraph& dg, ObjectIdMap& obj_map, DFS_EdgeDataMap& datas )
{
    // 关联分支属性
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        EdgeData* pEdgeData = new EdgeData();
        pEdgeData->objId = obj_map[e];
        SetEdgeType( pEdgeData->objId, pEdgeData );
        datas[e] = pEdgeData; // 关联一个Digraph::Arc属性
    }
}

DfsNetWork::DfsNetWork( void ) : datas( dg ), ef( dg, true ), bInitNetwokState( true )
{
    // 初始化网络以及添加处理源汇
    if( !initNetwok() )
    {
        ClearGraph( dg, datas );
        setInitNetwokState( false );
    }
    else
    {
        // 初始化用风分支
        initAirEdges();
    }
}

DfsNetWork::~DfsNetWork( void )
{
    if( bInitNetwokState )
    {
        ClearGraph( dg, datas );
    }
}

bool DfsNetWork::initNetwok()
{
    ObjectIdMap om( dg );
    if( !BuildNetwork( dg, om ) ) return false;
    if( !AddVirtualSTNode( dg, om, sn, tn ) ) return false;
    FilterBlockEdges( dg, om, ef );

    // 关联分支属性数据
    InitEdgeDatas( dg, om, datas );

    return true;
}

bool DfsNetWork::isInitNetworkOk() const
{
    return bInitNetwokState;
}

void DfsNetWork::setInitNetwokState( bool bState )
{
    bInitNetwokState = bState;
}

void DfsNetWork::initAirEdges()
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( datas[e]->et == ET_USEAIR )
        {
            airEdges.append( e );
        }
    }
}

Digraph::Arc DfsNetWork::getEdgeById( const AcDbObjectId& objId )
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

AcDbObjectId DfsNetWork::getIdByEdge( Digraph::Arc e )
{
    if( e == INVALID ) return AcDbObjectId::kNull;
    // 应该判断分支是否包含在图中
    // 目前就假设e肯定是图的分支
    return datas[e]->objId;
}

void DfsNetWork::doDFS( Digraph::Arc se, Digraph::Arc te, EdgeArray& p )
{
    if( se == INVALID || te == INVALID ) return;

    Digraph::Node s = dg.source( se );
    Digraph::Node t = dg.target( te );

    Dfs<Digraph> aDfs( dg );
    if( aDfs.run( s, t ) )
    {
        typedef Path<Digraph> DPath;
        DPath dp = aDfs.path( t );

        for( DPath::ArcIt e( dp ); e != INVALID; ++e )
        {
            p.append( e );
        }
    }
}