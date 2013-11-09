#include "StdAfx.h"
#include "IdentifyDiagonalStructure.h"

#include "../MineGE/ConstData.h"

#include "../ArxHelper/ArxUtilHelper.h"
#include "../ArxHelper/ArxEntityHelper.h"

#include <lemon/adaptors.h>
#include <lemon/dfs.h>
#include <lemon/bfs.h>

#include <vector>
#include <algorithm>

typedef Undirector<Digraph> UGraph;
typedef UGraph::NodeMap<bool> UNodeFilter;
typedef FilterNodes<UGraph, UNodeFilter> NFUGraph;
typedef Path<UGraph> UPath;

typedef Digraph::ArcMap<int> DSMap;

// 图适配路线
// Digraph dg -> AFGraph dg-> UGraph ug -> UGraph ug

//#include "../Tool/MyRandom.h"
#include "../Tool/Timer.h"

#include "../GraphTool/Tool.h"

// 角联结构7元组
// 目前只定义4个支撑节点
struct DiagonalStructrue
{
    Digraph::Node s, t;  // 分流节点、汇流节点
    Digraph::Node l, r;  // 左关联点、右关联点

    DiagonalStructrue() : s( INVALID ), t( INVALID ), l( INVALID ), r( INVALID )
    {
        // 默认初始化为无效角联
    }

    bool operator==( const DiagonalStructrue& ds ) const
    {
        return ( s == ds.s ) && ( t == ds.t ) && ( l == ds.l ) && ( r == ds.r );
    }

    bool isValid() const
    {
        return ( s != INVALID ) && ( t != INVALID ) && ( l != INVALID ) && ( r != INVALID ) &&
               ( s != t ) && ( l != r ) && ( s != l ) && ( s != r ) && ( t != l ) && ( t != r );
    }
};

typedef AcArray<DiagonalStructrue> DSArray;

// 用于计算最近的分流或汇流节点
struct NodeDist
{
    Digraph::Node u;
    int dist;
    bool operator<( const NodeDist& nd ) const
    {
        return ( dist < nd.dist );
    }
};

typedef std::vector<NodeDist> NDArray;

static void ClearGraph( Digraph& dg, IDS_EdgeDataMap& datas )
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

static void InitEdgeDatas( Digraph& dg, ObjectIdMap& om, IDS_EdgeDataMap& datas )
{
    // 关联分支属性
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        EdgeData* pEdgeData = new EdgeData();
        pEdgeData->objId = om[e];
        SetEdgeType( om[e], pEdgeData );
        datas[e] = pEdgeData; // 关联一个Digraph::Arc属性
    }
}

IdentifyDiagonalStructure::IdentifyDiagonalStructure( void )
    : datas( dg ), ef( dg, true ), ngm( dg ), bInitNetwokState( true )
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

IdentifyDiagonalStructure::~IdentifyDiagonalStructure( void )
{
    if( bInitNetwokState )
    {
        ClearGraph( dg, datas );
    }
}

bool IdentifyDiagonalStructure::initNetwok()
{
    ObjectIdMap om( dg );
    if( !BuildNetwork( dg, om ) ) return false;
    if( !AddVirtualSTNode( dg, om, sn, tn ) ) return false;
    if( !GetNodeGeoData( dg, om, ngm ) ) return false; // 虚拟源点和汇点的坐标为原点(0,0,0)
    FilterBlockEdges( dg, om, ef );

    // 删除阻断分支
    EraseBlockEdges( dg, ef );

    // 关联分支属性数据
    InitEdgeDatas( dg, om, datas );

    return true;
}

bool IdentifyDiagonalStructure::isInitNetworkOk() const
{
    return bInitNetwokState;
}

void IdentifyDiagonalStructure::setInitNetwokState( bool bState )
{
    bInitNetwokState = bState;
}

void IdentifyDiagonalStructure::initAirEdges()
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( datas[e]->et == ET_USEAIR )
        {
            airEdges.append( e );
        }
    }
}

Digraph::Arc IdentifyDiagonalStructure::getEdgeById( const AcDbObjectId& objId )
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

AcDbObjectId IdentifyDiagonalStructure::getIdByEdge( Digraph::Arc e )
{
    if( e == INVALID ) return AcDbObjectId::kNull;
    // 应该判断分支是否包含在图中
    // 目前就假设e肯定是图的分支
    return datas[e]->objId;
}

// 求s->u路径上的所有节点
static void FindNodesInST( UGraph& ug,
                           UGraph::Node s, UGraph::Node t,
                           UGraph::Node u, UGraph::Node v,
                           NodeArray& ns )
{
    typedef UGraph::ArcMap<bool> UArcFilter;
    UArcFilter af( ug, true );

    for( UGraph::OutArcIt ue( ug, v ); ue != INVALID; ++ue )
    {
        af[ue] = false;
    }
    for( UGraph::InArcIt ue( ug, v ); ue != INVALID; ++ue )
    {
        af[ue] = false;
    }
    for( UGraph::OutArcIt ue( ug, t ); ue != INVALID; ++ue )
    {
        af[ue] = false;
    }
    for( UGraph::InArcIt ue( ug, t ); ue != INVALID; ++ue )
    {
        af[ue] = false;
    }

    typedef FilterArcs<UGraph, UArcFilter> AFUGraph;
    AFUGraph afug( ug, af );

    // 标记节点是否在路径s->u中
    UNodeFilter nf( ug, true );

    bool bQuit = false;
    while( !bQuit )
    {
        bQuit = true;
        Bfs<AFUGraph> aBfs( afug );
        if( aBfs.run( s, u ) ) // u->s
        {
            bQuit = false;
            UPath p = aBfs.path( u );
            for( UPath::ArcIt ue( p ); ue != INVALID; ++ue )
            {
                af[ue] = false;
                nf[ug.source( ue )] = false; // 标记路径分支的始节点
            }
        }
    }

    for( UGraph::NodeIt n( ug ); n != INVALID; ++n )
    {
        if( !nf[n] ) ns.append( n );
    }
}

static void GetIntersectNodes( NodeArray& ns1, NodeArray& ns2, NodeArray& ns )
{
    // 直接使用最原始的搜索方法
    // 如果确实影响到运行效率，后期再考虑优化,使用set等其他算法
    int len = ns1.length();
    for( int i = 0; i < len; i++ )
    {
        if( ns2.contains( ns1[i] ) )
        {
            ns.append( ns1[i] );
        }
    }
}

static UGraph::Node GetMaxDistNodes( UGraph& ug,
                                     UGraph::Node s, UGraph::Node t,
                                     UGraph::Node u, UGraph::Node v,
                                     NodeArray& ns )
{
    if( ns.isEmpty() ) return INVALID;

    // 过滤节点
    UNodeFilter nf( ug, true );

    // 过滤节点后的无向图
    NFUGraph nfug( ug, nf );

    // 封闭汇点
    nf[t] = false;
    nf[v] = false;

    Bfs<NFUGraph> aBfs( nfug );
    aBfs.run( u ); // 从节点s开始遍历整个图
    //if(!aBfs.reached(u)) return INVALID;

    NDArray nds;

    int len = ns.length();
    for( int i = 0; i < len; i++ )
    {
        UGraph::Node uu = ns[i];
        if( !aBfs.reached( uu ) ) continue;
        if( aBfs.dist( uu ) == 0 ) continue;

        NodeDist nd;
        nd.u = uu;
        nd.dist = aBfs.dist( uu );
        nds.push_back( nd );
    }

    if( nds.empty() ) return INVALID;

    NDArray::iterator itr = std::min_element( nds.begin(), nds.end() );
    if( itr == nds.end() ) return INVALID;

    return itr->u;
}

// 确定最近的分流/汇流节点
// the nearest source and target
static UGraph::Node FindNST( UGraph& ug,
                             UGraph::Node s, UGraph::Node t,
                             UGraph::Node u, UGraph::Node v )
{
    // s->u的所有路径之间的节点
    NodeArray ns1;
    FindNodesInST( ug, s, t, u, v, ns1 );
    if( ns1.isEmpty() ) return INVALID;

    // s->v的所有路径之间的节点
    NodeArray ns2;
    FindNodesInST( ug, s, t, v, u, ns2 );
    if( ns2.isEmpty() ) return INVALID;

    // 计算交点
    NodeArray ns;
    GetIntersectNodes( ns1, ns2, ns );

    // 返回距离s->u或s->v最近的节点
    return GetMaxDistNodes( ug, s, t, u, v, ns );
}

static bool FindPath( UGraph& ug,
                      UNodeFilter& nf,
                      UGraph::Node u, UGraph::Node v,
                      UPath& p )
{
    // 过滤节点后的无向图
    NFUGraph nfug( ug, nf );

    // 第1次bfs
    Bfs<NFUGraph> aBfs( nfug );

    // 查找u->s的路径
    bool ret = aBfs.run( u, v ); // u->s

    // 获取路径p
    if( ret )
    {
        p = aBfs.path( v );
    }

    return ret;
}

static bool FindPath( UGraph& ug,
                      UGraph::Node s, UGraph::Node t,
                      UGraph::Node u, UGraph::Node v,
                      UPath& p1, UPath& p2 )
{
    // 过滤节点
    UNodeFilter nf1( ug, true );
    nf1[v] = false;
    nf1[t] = false;

    bool ret = true;
    while( true )
    {
        nf1[s] = true;
        nf1[u] = true;

        if( !FindPath( ug, nf1, u, s, p1 ) ) // u->s
        {
            acutPrintf( _T( "\nu->s查找失败" ) );
            ret = false;
            break;
        }

        UNodeFilter nf2( ug, true );
        nf2[u] = false;
        nf2[s] = false;

        for( UPath::ArcIt ue( p1 ); ue != INVALID; ++ue )
        {
            nf2[ug.target( ue )] = false;
            //nf1[ug.target(ue)] = false;
        }

        //nf1[ug.source(p1.back())] = false;
        nf1[ug.source( p1.back() )] = false;

        if( FindPath( ug, nf2, v, t, p2 ) ) // v->t
        {
            acutPrintf( _T( "\nv->t查找成功" ) );
            break;
        }
    }

    return ret;
}

static UGraph::Node GetNode( UGraph& ug, UGraph::Arc ue, bool startOrEnd )
{
    return ( startOrEnd ? ug.source( ue ) : ug.target( ue ) );
}

// 求路径p1和p2的交点
// 如果bNear=true，求最近的交点，那么考虑分支的末节点
// 如果bNear=false，求最远的交点，那么考虑分支的始节点
static UGraph::Node GetIntersectNode( UGraph& ug, UPath& p1, UPath& p2, bool bNear )
{
    UGraph::Node n = INVALID;

    // Digraph::Node 和 UGraph::Node是等价的
    // 使用栈将路径进行翻转
    NodeArray ns1;
    for( UPath::ArcIt ue( p1 ); ue != INVALID; ++ue )
    {
        ns1.append( GetNode( ug, ue, !bNear ) );
    }

    NodeArray ns2;
    for( UPath::ArcIt ue( p2 ); ue != INVALID; ++ue )
    {
        ns2.append( GetNode( ug, ue, !bNear ) );
    }

    int len = ns1.length();
    if( !bNear )
    {
        // 反向查找最远交点
        for( int i = len - 1; i >= 0; i-- )
        {
            if( ns2.contains( ns1[i] ) )
            {
                n = ns1[i];
                break;
            }
        }
    }
    else
    {
        // 正向查找最近交点
        for( int i = 0; i < len; i++ )
        {
            if( ns2.contains( ns1[i] ) )
            {
                n = ns1[i];
                break;
            }
        }
    }
    return n;
}

static void FindAllDS( Digraph& dg, Digraph::Node s, Digraph::Node t, DSMap& ds_map, DSArray& dss )
{
    // 转换成无向图
    UGraph ug( dg );

    // 后续操作都使用无向图进行操作
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        Digraph::Node u = dg.source( e ); // 始节点
        Digraph::Node v = dg.target( e ); // 末节点

        if( u == s || v == s || u == t || v == t ) continue;

        // 确定最近的分流节点
        //Digraph::Node ns = FindNST(ug, s, t, u, v); // u->s and v->s
        //if(ns == INVALID) continue;

        // 确定最近的汇流节点
        //Digraph::Node nt = FindNST(ug, t, s, u, v); // u->t and v->t
        //if(nt == INVALID) continue;

        // 查找s->u->v->t的路径
        // p1={u->s}, p2={v->t}
        UPath p1, p2;
        if( !FindPath( ug, s, t, u, v, p1, p2 ) ) continue;

        // 查找s->v->u->t的路径
        // p3={v->s} p4={u->t}
        UPath p3, p4;
        if( !FindPath( ug, s, t, v, u, p3, p4 ) ) continue;

        // 对路径进行分析，确定角联结构的4个支撑点
        DiagonalStructrue ds;

        // 分析p1和p3确定分流节点(比较路径分支的末节点)
        ds.s = GetIntersectNode( ug, p1, p3, true );
        //ds.s = ns;

        // 分析p2和p4确定汇流节点(比较路径分支的末节点)
        ds.t = GetIntersectNode( ug, p2, p4, true );
        //ds.t = nt;

        // 分析p1和p4确定左关联点(比较路径分支的始节点)
        ds.l = GetIntersectNode( ug, p1, p4, false );

        // 分析p2和p3确定右关联点(比较路径分支的始节点)
        ds.r = GetIntersectNode( ug, p2, p3, false );

        // 该角联结构是有效的
        if( ds.isValid() )
        {
            // 添加到角联结构数组中
            int pos = dss.find( ds );
            if( pos == -1 ) pos = dss.append( ds ); // pos记录添加的元素的索引位置
            ds_map[e] = pos;
        }
    }
}

static void PrintDS( DiagonalStructrue& ds, NodeGeoDataMap& ngm )
{
    AcGePoint3d pt = ngm[ds.s];
    acutPrintf( _T( "\n分流点坐标:(%.3f, %.3f, %.3f)" ), pt.x, pt.y, pt.z );

    pt = ngm[ds.t];
    acutPrintf( _T( "\n汇流点坐标:(%.3f, %.3f, %.3f)" ), pt.x, pt.y, pt.z );

    pt = ngm[ds.l];
    acutPrintf( _T( "\n左关联点坐标:(%.3f, %.3f, %.3f)" ), pt.x, pt.y, pt.z );

    pt = ngm[ds.r];
    acutPrintf( _T( "\n右关联点坐标:(%.3f, %.3f, %.3f)" ), pt.x, pt.y, pt.z );
}

static bool FindDS( Digraph& dg,
                    NodeGeoDataMap& ngm,
                    Digraph::Node s, Digraph::Node t,
                    Digraph::Arc e, DiagonalStructrue& ds )
{
    Digraph::Node u = dg.source( e ); // 始节点
    Digraph::Node v = dg.target( e ); // 末节点

    if( u == s || v == s || u == t || v == t ) return false;

    // 转换成无向图
    UGraph ug( dg );

    // 确定最近的分流节点
    Digraph::Node ns = FindNST( ug, s, t, u, v ); // u->s and v->s
    if( ns == INVALID )
    {
        acutPrintf( _T( "\n无法找到分流节点..." ) );
        return false;
    }

    // 确定最近的汇流节点
    //Digraph::Node nt = FindNST(ug, t, s, u, v); // u->t and v->t
    Digraph::Node nt = FindNST( ug, t, ns, u, v ); // u->t and v->t
    if( nt == INVALID )
    {
        acutPrintf( _T( "\n无法找到汇流节点..." ) );
        return false;
    }

    if( ns == nt )
    {
        acutPrintf( _T( "\n搜索错误!!!查找到的分流节点和汇流节点相同!!!" ) );
        return false;
    }

    // 查找s->u->v->t的路径
    // p1={u->s}, p2={v->t}
    UPath p1, p2;
    if( !FindPath( ug, ns, nt, u, v, p1, p2 ) )
    {
        acutPrintf( _T( "\n查找路径s->u->v->t失败..." ) );
        return false;
    }

    // 查找s->v->u->t的路径
    // p3={v->s} p4={u->t}
    UPath p3, p4;
    if( !FindPath( ug, ns, nt, v, u, p3, p4 ) )
    {
        acutPrintf( _T( "\n查找路径s->v->u->t失败..." ) );
        return false;
    }

    // 对路径进行分析，确定角联结构的4个支撑点

    // 分析p1和p3确定分流节点(比较路径分支的末节点)
    ds.s = GetIntersectNode( ug, p1, p3, true );

    // 分析p2和p4确定汇流节点(比较路径分支的末节点)
    ds.t = GetIntersectNode( ug, p2, p4, true );

    // 分析p1和p4确定左关联点(比较路径分支的始节点)
    ds.l = GetIntersectNode( ug, p1, p4, false );

    // 分析p2和p3确定右关联点(比较路径分支的始节点)
    ds.r = GetIntersectNode( ug, p2, p3, false );

    PrintDS( ds, ngm );

    // 该角联结构是有效的
    return ds.isValid();
}

// 从无向图分支转换成有向图分支
static Digraph::Arc UArc2Arc( UGraph::Arc ue )
{
    Digraph::Arc e = ue;
    return e;
}

static bool GetDS( Digraph::Arc e, DSMap& ds_map, DSArray& dss, DiagonalStructrue& ds )
{
    // 如果等于-1，则该分支不是角联分支
    int pos = ds_map[e];
    if( pos == -1 ) return false;

    // 获取角联结构
    ds = dss[pos];

    return true;
}

static bool FindDSPath( Digraph& dg, DiagonalStructrue& ds, EdgeArray& pp1, EdgeArray& pp2 )
{
    // 转换成无向图
    UGraph ug( dg );

    // 查找左右关联通路
    UPath p1, p2;
    if( !FindPath( ug, ds.s, ds.t, ds.l, ds.r, p1, p2 ) ) return false;

    UPath p3, p4;
    if( !FindPath( ug, ds.s, ds.t, ds.r, ds.l, p3, p4 ) ) return false;

    // p1和p4合并为左关联通路
    for( UPath::ArcIt ue( p1 ); ue != INVALID; ++ue )
    {
        pp1.append( UArc2Arc( ue ) );
    }
    for( UPath::ArcIt ue( p2 ); ue != INVALID; ++ue )
    {
        pp1.append( UArc2Arc( ue ) );
    }

    // p2和p3合并为右关联通路
    for( UPath::ArcIt ue( p3 ); ue != INVALID; ++ue )
    {
        pp2.append( UArc2Arc( ue ) );
    }
    for( UPath::ArcIt ue( p4 ); ue != INVALID; ++ue )
    {
        pp2.append( UArc2Arc( ue ) );
    }

    return true;
}

static bool IDS_Helper3( Digraph& dg, Digraph::Node sn, Digraph::Node tn, Digraph::Arc e, EdgeArray& pp1, EdgeArray& pp2 )
{
    // 记录各分支所对应的角联结构
    DSMap ds_map( dg );
    for( Digraph::ArcIt ee( dg ); ee != INVALID; ++ee )
    {
        ds_map[ee] = -1; // 默认为无效角联
    }

    // 如果使用UGraph::ArcIt遍历无向图的分支
    // 每条分支都会被遍历2次(正向和反向)

    // 角联结构数组
    DSArray dss;

    // 查找所有的角联结构
    FindAllDS( dg, sn, tn, ds_map, dss );

    // 获取角联结构
    DiagonalStructrue ds;
    if( !GetDS( e, ds_map, dss, ds ) ) return false;

    return FindDSPath( dg, ds, pp1, pp2 );
}

static bool IDS_Helper( Digraph& dg, Digraph::Node sn, Digraph::Node tn, NodeGeoDataMap& ngm, Digraph::Arc e, EdgeArray& pp1, EdgeArray& pp2, bool findNST = false )
{
    // 查找分支对应的角联结构
    DiagonalStructrue ds;
    if( findNST )
    {
        if( !FindDS( dg, ngm, sn, tn, e, ds ) ) return false;
    }
    else
    {
        ds.s = sn;
        ds.t = tn;
        ds.l = dg.source( e );
        ds.r = dg.target( e );
    }

    AcGePoint3d pt = ngm[ds.s];
    acutPrintf( _T( "\n分流点坐标:(%.3f, %.3f, %.3f)" ), pt.x, pt.y, pt.z );

    pt = ngm[ds.t];
    acutPrintf( _T( "\n汇流点坐标:(%.3f, %.3f, %.3f)" ), pt.x, pt.y, pt.z );

    return FindDSPath( dg, ds, pp1, pp2 );
}

bool IdentifyDiagonalStructure::isDS( const AcDbObjectId& objId )
{
    if( !isInitNetworkOk() ) return false;

    Digraph::Arc e = getEdgeById( objId );
    if( e == INVALID ) return false;
    if( !ef[e] ) return false; // 阻断分支

    Digraph::Node u = dg.source( e );
    Digraph::Node v = dg.target( e );

    UGraph ug( dg );

    // 查找左右关联通路
    UPath p1, p2;
    if( !FindPath( ug, sn, tn, u, v, p1, p2 ) )
    {
        acutPrintf( _T( "\n查找路径s->u->v->t失败..." ) );
        return false;
    }

    UPath p3, p4;
    if( !FindPath( ug, sn, tn, v, u, p3, p4 ) )
    {
        acutPrintf( _T( "\n查找路径s->v->u->t失败..." ) );
        return false;
    }

    return true;
}

// findNST -- 是否查找最近的分流节点和汇流节点
bool IdentifyDiagonalStructure::doIDS( const AcDbObjectId& objId, AcDbObjectIdArray& p1, AcDbObjectIdArray& p2, bool findNST )
{
    if( !isInitNetworkOk() ) return false;

    Digraph::Arc e = getEdgeById( objId );
    if( e == INVALID )
    {
        acutPrintf( _T( "\n该分支不存在" ) );
        return false;
    }

    acutPrintf( _T( "\n开始识别角联结构..." ) );
    EdgeArray pp1, pp2;
    if( !IDS_Helper( dg, sn, tn, ngm, e, pp1, pp2, findNST ) ) return false;

    acutPrintf( _T( "\n获取左右关联通路的实体id..." ) );
    int len = pp1.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = pp1[i];
        if( datas[e]->et == ET_VIRTUAL ) continue;
        if( p1.contains( datas[e]->objId ) ) continue;

        p1.append( datas[e]->objId );
    }

    /*int*/ len = pp2.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = pp2[i];
        if( datas[e]->et == ET_VIRTUAL ) continue;
        if( p2.contains( datas[e]->objId ) ) continue;

        p2.append( datas[e]->objId );
    }

    // 查找角联结构对应的所有分支
    return true;
}
