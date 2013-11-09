#include "StdAfx.h"
#include "IdentifyLiaisonTunnel.h"

#include "../MineGE/ConstData.h"

#include "../ArxHelper/ArxUtilHelper.h"
#include "../ArxHelper/ArxEntityHelper.h"

#include <lemon/adaptors.h>
#include <lemon/dfs.h>
#include <lemon/bfs.h>

#include <vector>
#include <algorithm>

typedef FilterArcs<Digraph, ArcFilter> AFGraph;
typedef ReverseDigraph<AFGraph> RDigraph;
typedef Digraph::NodeMap<int> IntMap;

#include "../GraphTool/Tool.h"

extern void DisplayIntValue2( const AcDbObjectIdArray& objIds, const AcDbIntArray& ivs, const AcDbIntArray& ivs2 );
extern void DisplayIntValue( const AcDbObjectIdArray& objIds, const AcDbIntArray& ivs );

//#include "../Tool/MyRandom.h"
#include "../Tool/Timer.h"

struct AirEdgeDist
{
    AirEdgeDist() : sd( 0 ), td( 0 ) {}
    Digraph::Arc e;
    int sd, td;
    bool operator<( const AirEdgeDist& aed ) const
    {
        if( sd == aed.sd )
            return ( td > aed.td );
        else
            return ( sd > aed.sd );
    }
};

// 标记用风地点的始末节点和网络源汇节点
// 在搜索的时候，不搜索这些节点
static void BuildNodeFilter( Digraph& dg, EdgeArray& airEdges, Digraph::Node s, Digraph::Node t, NodeFilter& nf )
{
    int len = airEdges.length();
    acutPrintf( _T( "\n用风地点个数:%d" ), len );
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = airEdges[i];
        nf[dg.source( e )] = false;
        nf[dg.target( e )] = false;
    }

    nf[s] = false;
    nf[t] = false;

    // 原始网络的源点和汇点也要标记
    for( Digraph::OutArcIt e( dg, s ); e != INVALID; ++e )
    {
        nf[dg.target( e )] = false;
    }
    for( Digraph::InArcIt e( dg, t ); e != INVALID; ++e )
    {
        nf[dg.source( e )] = false;
    }
}

// 使用宽度优先搜索BFS计算最短距离
static void SortAirEdges( Digraph& dg, ArcFilter& ef, Digraph::Node s, Digraph::Node t, const EdgeArray& airEdges, EdgeArray& es )
{
    AFGraph afg( dg, ef );

    Bfs<AFGraph> aBfs( afg );
    aBfs.run( s );
    //if(!aDfs.reached(t)) return; // s->t不可达

    typedef ReverseDigraph<AFGraph> RDigraph;
    RDigraph rdg( afg );
    Bfs<RDigraph> bBfs( rdg );
    bBfs.run( t );
    //if(!bDfs.reached(s)) return;

    typedef std::vector<AirEdgeDist> AirEdgeArray;
    AirEdgeArray aes;

    int len = airEdges.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = airEdges[i];
        Digraph::Node u = dg.source( e );
        Digraph::Node v = dg.target( e );

        AirEdgeDist aed;
        aed.e = e;
        if( aBfs.reached( u ) )
        {
            aed.sd = aBfs.dist( u );
        }
        else
        {
            acutPrintf( _T( "\n第%d个用风地点【始点】不可达" ), i );
        }

        if( bBfs.reached( v ) )
        {
            aed.td = bBfs.dist( v );
        }
        else
        {
            acutPrintf( _T( "\n第%d个用风地点【末点】不可达" ), i );
        }

        aes.push_back( aed );
    }

    // 降序排序
    std::sort( aes.begin(), aes.end() );

    for( AirEdgeArray::iterator itr = aes.begin(); itr != aes.end(); itr++ )
    {
        es.append( itr->e );
    }
}

static void Init_IMap( Digraph& dg, ArcFilter& ef, NodeFilter& nf, EdgeArray& airEdges, IntMap& imap )
{
    AFGraph afg( dg, ef );

    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        imap[n] = ( nf[n] ? 1 : 0 ); // 默认所有节点位于进风区
    }

    int len = airEdges.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = airEdges[i];
        Digraph::Node u = dg.source( e );
        Digraph::Node v = dg.target( e );

        Dfs<AFGraph> bDfs( afg );
        bDfs.run( v );
        for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
        {
            if( nf[n] && bDfs.reached( n ) ) imap[n] = -1; // 节点位于回风区
        }
    }
}

static void FilterILTEdge( Digraph& dg, IntMap& imap, ArcFilter& ef )
{
    // 查找联络巷
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        Digraph::Node u = dg.source( e );
        Digraph::Node v = dg.target( e );

        // 排除直接相连的分支
        // 联络巷分支的始节点出度必须大于1，末节点的入度必须大于1
        if( countOutArcs( dg, u ) <= 1 || countInArcs( dg, v ) <= 1 ) continue;

        if( imap[u]*imap[v] < 0 ) ef[e] = false;
    }
}

// cmap表示属于用风地点的次数
// 如果大于0，则表示多个用风地点的进风路线经过该节点
// 如果小于0，则表示多个用风地点的回风路线经过该节点
static void Init_CMap( Digraph& dg, IntMap& cmap )
{
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        cmap[n] = 0;
    }
}

static int CountInFlowNodes( Digraph& dg, ArcFilter& ef, NodeFilter& nf, Digraph::Node u, IntMap& cmap )
{
    AFGraph afg( dg, ef );
    // 反向图
    RDigraph rdg( afg );

    int minCount = 1000;
    Dfs<RDigraph> aDfs( rdg );
    aDfs.run( u ); // dfs遍历所有节点
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        if( nf[n] && aDfs.reached( n ) )
        {
            cmap[n] = cmap[n] + 1;
            if( minCount > cmap[n] ) minCount = cmap[n];
        }
    }
    return minCount;
}

static int CountOutFlowNodes( Digraph& dg, ArcFilter& ef, NodeFilter& nf, Digraph::Node v, IntMap& cmap )
{
    AFGraph afg( dg, ef );

    int maxCount = -1000;
    Dfs<AFGraph> bDfs( afg );
    bDfs.run( v );
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        if( nf[n] && bDfs.reached( n ) )
        {
            cmap[n] = cmap[n] - 1;
            if( maxCount < cmap[n] ) maxCount = cmap[n];
        }
    }
    return maxCount;
}

static void MinusInFlowNodes( Digraph& dg, ArcFilter& ef, NodeFilter& nf, Digraph::Node u, IntMap& cmap, int minCount )
{
    AFGraph afg( dg, ef );
    // 反向图
    RDigraph rdg( afg );

    Dfs<RDigraph> aDfs( rdg );
    aDfs.run( u ); // dfs遍历所有节点
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        if( nf[n] && aDfs.reached( n ) && ( cmap[n] == minCount ) )
        {
            cmap[n] = cmap[n] - 1;
        }
    }
}

static void PlusOutFlowNodes( Digraph& dg, ArcFilter& ef, NodeFilter& nf, Digraph::Node v, IntMap& cmap, int maxCount )
{
    AFGraph afg( dg, ef );

    // 正向dfs
    Dfs<AFGraph> bDfs( afg );
    bDfs.run( v );
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        if( nf[n] && bDfs.reached( n ) && ( cmap[n] == maxCount ) )
        {
            cmap[n] = cmap[n] + 1;
        }
    }
}

static void FindILTEdges( Digraph& dg, ILT_EdgeDataMap2& datas, IntMap& cmap, EdgeArray& edges )
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( datas[e]->et == ET_VIRTUAL ) continue;
        Digraph::Node u = dg.source( e );
        Digraph::Node v = dg.target( e );

        // 排除直接相连的分支
        // 联络巷分支的始节点出度必须大于1，末节点的入度必须大于1
        if( countOutArcs( dg, u ) <= 1 || countInArcs( dg, v ) <= 1 ) continue;

        if( cmap[u]*cmap[v] < 0 ) edges.append( e );
    }
}

static void DisplayAirEdgeNum( EdgeArray& airEdges, ILT_EdgeDataMap2& datas )
{
    AcDbObjectIdArray objIds2;
    AcDbIntArray ivs3;

    int len = airEdges.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = airEdges[i];
        objIds2.append( datas[e]->objId );
        ivs3.append( i );
    }
    DisplayIntValue( objIds2, ivs3 );
}

static void DisplayEdgeCMapValue( Digraph& dg, EdgeArray& sortedAirEdges,
                                  ILT_EdgeDataMap2& datas, IntMap& cmap,
                                  AcDbIntArray& sAirs, AcDbIntArray& tAirs )
{
    AcDbObjectIdArray objIds;
    AcDbIntArray ivs, ivs2;
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        objIds.append( datas[e]->objId );
        int pos = sortedAirEdges.find( e );
        if( pos != -1 )
        {
            ivs.append( sAirs[pos] );
            ivs2.append( tAirs[pos] );
        }
        else
        {
            ivs.append( cmap[dg.source( e )] );
            ivs2.append( cmap[dg.target( e )] );
        }
    }
    DisplayIntValue2( objIds, ivs, ivs2 );
}

static void ILT_Helper( Digraph& dg, ILT_EdgeDataMap2& datas, ArcFilter& ef, NodeFilter& nf, EdgeArray& sortedAirEdges, IntMap& cmap )
{
    // 记录用风地点的始末搜索得到的count
    AcDbIntArray sAirs, tAirs;

    double st = Timer::rdtscSeconds();

    // 显示排序后，用风地点的顺序编号
    //DisplayAirEdgeNum(sortedAirEdges, datas);

    int len = sortedAirEdges.length();
    acutPrintf( _T( "\n用风地点个数:%d" ), len );
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = sortedAirEdges[i];
        Digraph::Node u = dg.source( e );
        Digraph::Node v = dg.target( e );

        sAirs.append( CountInFlowNodes( dg, ef, nf, u, cmap ) );
        tAirs.append( CountOutFlowNodes( dg, ef, nf, v, cmap ) );
        //acutPrintf(_T("\n第%d个用风地点【始节点】=%d  【末节点】=%d"), i, sAirs[i], tAirs[i]);
    }

    /*for(Digraph::NodeIt n(dg); n!=INVALID; ++n)
    {
    	if(nf[n] && imap[n]==0) imap[n] = 1;
    }*/

    double et = Timer::rdtscSeconds();
    acutPrintf( _T( "\n第1次正反dfs查找耗费时间:%.5f" ), et - st );

    // 显示分支的始末节点的cmap值(测试用)
    //DisplayEdgeCMapValue(dg, sortedAirEdges, datas, cmap, sAirs, tAirs);

    //assert(sAirs.length() == len);
    //assert(tAirs.length() == len);

    st = Timer::rdtscSeconds();

    for( int i = len - 1; i >= 0; i-- )
    {
        Digraph::Arc e = sortedAirEdges[i];
        Digraph::Node u = dg.source( e );
        Digraph::Node v = dg.target( e );

        MinusInFlowNodes( dg, ef, nf, u, cmap, sAirs[i] );
        PlusOutFlowNodes( dg, ef, nf, v, cmap, tAirs[i] );
    }

    et = Timer::rdtscSeconds();
    acutPrintf( _T( "\n第2次正反dfs查找耗费时间:%.5f" ), et - st );

    /*
     * 处理完之后，可能出现一些分支的始节点(或末节点)的cmap=0
     * 而末节点(或始节点)的cmap不等于0
     * 这些分支中可能有部分分支也可视为主要进回风之间的联络巷
     */
    // 显示分支的始末节点的cmap值(测试用)
    //DisplayEdgeCMapValue(dg, sortedAirEdges, datas, cmap, sAirs, tAirs);
}

static void ClearGraph( Digraph& dg, ILT_EdgeDataMap2& datas )
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

static void InitEdgeDatas( Digraph& dg, ObjectIdMap& om, ILT_EdgeDataMap2& datas )
{
    // 关联分支属性
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        EdgeData* pEdgeData = new EdgeData();
        pEdgeData->objId = om[e];
        SetEdgeType( pEdgeData->objId, pEdgeData );
        datas[e] = pEdgeData; // 关联一个Digraph::Arc属性
    }
}

IdentifyLiaisonTunnel::IdentifyLiaisonTunnel( void ) : datas( dg ), ef( dg, true ), bInitNetwokState( true )
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

IdentifyLiaisonTunnel::~IdentifyLiaisonTunnel( void )
{
    if( bInitNetwokState )
    {
        ClearGraph( dg, datas );
    }
}

bool IdentifyLiaisonTunnel::initNetwok()
{
    ObjectIdMap om( dg );
    if( !BuildNetwork( dg, om ) ) return false;
    FilterBlockEdges( dg, om, ef );
    if( !AddVirtualSTNode( dg, om, sn, tn ) ) return false;

    // 关联分支属性数据
    InitEdgeDatas( dg, om, datas );

    return true;
}

bool IdentifyLiaisonTunnel::isInitNetworkOk() const
{
    return bInitNetwokState;
}

void IdentifyLiaisonTunnel::setInitNetwokState( bool bState )
{
    bInitNetwokState = bState;
}

void IdentifyLiaisonTunnel::initAirEdges()
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( datas[e]->et == ET_USEAIR )
        {
            airEdges.append( e );
        }
    }
}

Digraph::Arc IdentifyLiaisonTunnel::getEdgeById( const AcDbObjectId& objId )
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

AcDbObjectId IdentifyLiaisonTunnel::getIdByEdge( Digraph::Arc e )
{
    if( e == INVALID ) return AcDbObjectId::kNull;
    // 应该判断分支是否包含在图中
    // 目前就假设e肯定是图的分支
    return datas[e]->objId;
}

bool IdentifyLiaisonTunnel::doILT2( EdgeArray& edges )
{
    if( !isInitNetworkOk() ) return false;
    if( airEdges.isEmpty() ) return false;

    NodeFilter nf( dg, true );
    BuildNodeFilter( dg, airEdges, sn, tn, nf );

    // 记录节点属于哪个区域(0-用风区, 1-进风区, -1-回风区)
    IntMap imap( dg );
    Init_IMap( dg, ef, nf, airEdges, imap );

    // 查找联络巷
    FindILTEdges( dg, datas, imap, edges );

    return true;
}

bool IdentifyLiaisonTunnel::doILT3( EdgeArray& edges )
{
    if( !isInitNetworkOk() ) return false;
    if( airEdges.isEmpty() ) return false;

    NodeFilter nf( dg, true );
    BuildNodeFilter( dg, airEdges, sn, tn, nf );

    double st = Timer::rdtscSeconds();

    double et = Timer::rdtscSeconds();
    acutPrintf( _T( "\n过滤阻断分支耗费时间:%.5f" ), et - st );

    IntMap cmap( dg );
    Init_CMap( dg, cmap );

    st = Timer::rdtscSeconds();

    // 对用风地点按距离进行排序
    EdgeArray sortedAirEdges;
    SortAirEdges( dg, ef, sn, tn, airEdges, sortedAirEdges );

    et = Timer::rdtscSeconds();
    acutPrintf( _T( "\n对用风地点按距离排序耗费时间:%.5f" ), et - st );

    ILT_Helper( dg, datas, ef, nf, sortedAirEdges, cmap );

    st = Timer::rdtscSeconds();

    // 查找联络巷
    FindILTEdges( dg, datas, cmap, edges );

    et = Timer::rdtscSeconds();
    acutPrintf( _T( "\n查找联络巷耗费时间:%.5f" ), et - st );

    return true;
}

static void InitDegMap( Digraph& dg, ArcFilter& ef, IntMap& inDeg, IntMap& outDeg )
{
    AFGraph afg( dg, ef );

    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        inDeg[n] = countInArcs( afg, n );
        outDeg[n] = countOutArcs( afg, n );
    }
}

bool IdentifyLiaisonTunnel::doILT4( EdgeArray& edges )
{
    if( !isInitNetworkOk() ) return false;
    if( airEdges.isEmpty() ) return false;

    NodeFilter nf( dg, true );
    BuildNodeFilter( dg, airEdges, sn, tn, nf );

    IntMap imap( dg );
    Init_IMap( dg, ef, nf, airEdges, imap );

    // 记录节点的入度和出度
    // 过滤阻断分支
    IntMap inDeg( dg ), outDeg( dg );
    InitDegMap( dg, ef, inDeg, outDeg );

    bool bQuit = false;
    while( !bQuit )
    {
        bQuit = true;

        for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
        {
            if( !ef[e] || datas[e]->et == ET_VIRTUAL ) continue;

            Digraph::Node u = dg.source( e );
            Digraph::Node v = dg.target( e );
            if( imap[u]*imap[v] <= 0 ) continue;
            if( outDeg[u] - 1 <= 0 || inDeg[v] - 1 <= 0 ) continue;

            if( imap[v] < 0 )
            {
                bool ret = false;
                // 回风区,考察节点的入边是否有新风流入
                for( Digraph::InArcIt ie( dg, v ); ie != INVALID; ++ie )
                {
                    if( imap[dg.source( ie )] > 0 )
                    {
                        ret = true;
                        break;
                    }
                }
                if( ret ) continue;
            }

            outDeg[u] = outDeg[u] - 1;
            inDeg[v] = inDeg[v] - 1;
            edges.append( e );
            bQuit = false;
        }
    }

    return true;
}

bool IdentifyLiaisonTunnel::findInOutEdges( Digraph::Arc le, EdgeArray& es1, EdgeArray& es2 )
{
    if( le == INVALID ) return false;

    for( Digraph::InArcIt e( dg, dg.source( le ) ); e != INVALID; ++e )
    {
        if( datas[e]->et == ET_VIRTUAL ) continue; // 排除处理掘进工作面时增加的虚拟分支
        es1.append( e );
    }
    for( Digraph::OutArcIt e( dg, dg.target( le ) ); e != INVALID; ++e )
    {
        if( datas[e]->et == ET_VIRTUAL ) continue; // 排除处理掘进工作面时增加的虚拟分支
        es2.append( e );
    }
    return true;
}