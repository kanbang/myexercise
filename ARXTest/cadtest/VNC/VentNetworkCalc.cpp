#include "VentNetworkCalc.h"

#include "config.h"
#include "IndependentCycle.h"

#include <lemon/adaptors.h>
#include <lemon/kruskal.h>
#include <lemon/dfs.h>

#include <cmath>
#include <iterator>

typedef Digraph::ArcMap<double> Weight;
typedef Digraph::ArcMap<bool> ArcFilter;

typedef std::vector<double> DValueArray;

static Digraph::Node AddVirtualSource( Digraph& dg )
{
    NodeArray sNodes;  // 源点集合

    // 对网络进行分析,查找源点
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        // 记录入度=0的源点
        if( 0 == countInArcs( dg, n ) ) sNodes.push_back( n );
    }

    /*
     * !!!前提条件!!!
     * 至少有一个源点和一个汇点
     * 目前暂时不考虑其它的"不正常情况"
     * 例如：没有源点或没有汇点，或者源汇点都没有
     */
    if( sNodes.empty() ) return INVALID;

    // 增加一个虚拟源点
    Digraph::Node sn = dg.addNode();

    // 增加源点"流出分支"(虚拟源点-->源点)
    int len = sNodes.size();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = dg.addArc( sn, sNodes[i] );
    }
    return sn;  // 返回新增的虚拟源点
}

static Digraph::Node AddVirtualTarget( Digraph& dg )
{
    NodeArray tNodes;  // 源点集合

    // 对网络进行分析,查找汇点
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        // 记录出度=0的汇点
        if( 0 == countOutArcs( dg, n ) ) tNodes.push_back( n );
    }

    /*
     * !!!前提条件!!!
     * 至少有一个源点和一个汇点
     * 目前暂时不考虑其它的"不正常情况"
     * 例如：没有源点或没有汇点，或者源汇点都没有
     */

    if( tNodes.empty() ) return INVALID;

    // 增加一个虚拟汇点
    Digraph::Node tn = dg.addNode();

    // 增加汇点"流入分支"(汇点-->虚拟汇点)
    int len = tNodes.size();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = dg.addArc( tNodes[i], tn );
    }
    return tn; // 返回新增的虚拟汇点
}

static bool AddVirtualSTNode( Digraph& dg, Digraph::Node& sn, Digraph::Node& tn )
{
    // 增加一个虚拟源点及分支
    /*Digraph::Node*/ sn = AddVirtualSource( dg );

    // 增加一个虚拟汇点及分支
    /*Digraph::Node*/
    tn = AddVirtualTarget( dg );

    return ( sn != INVALID ) && ( tn != INVALID );
}

static Digraph::Arc AddVirtualAirEdge( Digraph& dg, Digraph::Node sn, Digraph::Node tn )
{
    return dg.addArc( tn, sn );
}

static void DeleteVirtualSTEdge( Digraph& dg, Digraph::Node sn, Digraph::Node tn )
{
    EdgeArray es;
    for( Digraph::OutArcIt e( dg, sn ); e != INVALID; ++e )
    {
        es.push_back( e );
    }
    for( Digraph::InArcIt e( dg, tn ); e != INVALID; ++e )
    {
        es.push_back( e );
    }
    // 删除虚拟分支
    int n = es.size();
    for( int i = 0; i < n; i++ )
    {
        dg.erase( es[i] );
    }
}

static void DeleteVirtualAirEdge( Digraph& dg, Digraph::Arc e )
{
    dg.erase( e );
}

static void DeleteVirtualSTNode( Digraph& dg, Digraph::Node sn, Digraph::Node tn )
{
    // 删除虚拟源汇节点
    dg.erase( sn );
    dg.erase( tn );
}

static void InitVirtualSTEdgeData( Digraph& dg, Digraph::Node sn, Digraph::Node tn, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd )
{
    for( Digraph::OutArcIt e( dg, sn ); e != INVALID; ++e )
    {
        ed[e] = new VNC_EdgeData();
        fd[e] = 0;
    }
    for( Digraph::InArcIt e( dg, tn ); e != INVALID; ++e )
    {
        ed[e] = new VNC_EdgeData();
        fd[e] = 0;
    }
}

static void InitVirtualAirEdgeData( Digraph& dg, Digraph::Arc e, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd )
{
    ed[e] = new VNC_EdgeData();
    fd[e] = 0;
}

static void DeleteVirtualSTEdgeData( Digraph& dg, Digraph::Node sn, Digraph::Node tn, VNC_EdgeDataMap& ed )
{
    for( Digraph::OutArcIt e( dg, sn ); e != INVALID; ++e )
    {
        delete ed[e];
    }
    for( Digraph::InArcIt e( dg, tn ); e != INVALID; ++e )
    {
        delete ed[e];
    }
}

static void DeleteVirtualAirEdgeData( Digraph& dg, Digraph::Arc e, VNC_EdgeDataMap& ed )
{
    delete ed[e];
}

static double MaxAbsValue( DValueArray& dvalues )
{
    // 求绝对值最大的元素
    double maxD = abs( dvalues[0] );

    int n = dvalues.size();
    for( int i = 0; i < n; i++ )
    {
        double d = abs( dvalues[i] );
        if( ( maxD - d ) < 0 ) maxD = d;
    }
    return maxD;
}

static double AbsSquare( DValueArray& dvalues )
{
    // 所有数据的绝对值的均方差
    int n = dvalues.size();

    // 1、平均值
    double a = 0;
    for( int i = 0; i < n; i++ )
    {
        a += abs( dvalues[i] );
    }
    a = a / n;

    // 2、均方差
    double s = 0;
    for( int i = 0; i < n; i++ )
    {
        double v = abs( dvalues[i] );
        s += ( a - v ) * ( a - v );
    }
    return sqrt( s / n );
}

// 也可以参考"数理统计"中的一些误差计算方法
static bool IsMeetErrorRequirement( DValueArray& dvalues, double precise )
{
    if( dvalues.empty() ) return true;

    double maxD = MaxAbsValue( dvalues );

    bool ret = ( maxD - precise ) <= 0;

    //acutPrintf(_T("\n最大偏差值:%.5f, %s精度要求:%.5f\n"), maxD, (ret?_T("符合"):_T("不符合")), precise);

    // 比较"最大的数据的绝对值"与"精度"的大小
    return ret;
}

static double Magnify_Double( double v, double coeff )
{
    return ( 1 + v ) * coeff;
}

static double CaclIterateVar( VNC_EdgeDataMap& ed, VNC_FanDataMap& fd, bool speedUp, Digraph::Arc e )
{
    double r = ed[e]->r;
    double q = ed[e]->q;
    double fq = ed[e]->fq; // 固定风量

    // 放大固定风量分支和风机分支(作为余枝)
    double c = abs( speedUp ? r * q : r );

    if( fd[e] != 0 )
    {
        c = Magnify_Double( c, LARGE_COEFF );  // 风机分支
    }

    if( fq > 0 )
    {
        c = Magnify_Double( c, LARGE_COEFF ); // 固定风量分支
    }
    return c;
}

static void BuildWeight( Digraph& dg, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd, bool speedUp, Weight& w )
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        w[e] = CaclIterateVar( ed, fd, speedUp, e );
    }
}

// Tree Filter == > tf默认全部为false
static void BuildTreeFilter( EdgeArray& tree, ArcFilter& tf )
{
    int n = tree.size();
    for( int i = 0; i < n; i++ )
    {
        tf[tree[i]] = true;
    }
}

static void FindLeftTree( Digraph& dg, ArcFilter& tf, EdgeArray& leftTree )
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( !tf[e] )
        {
            leftTree.push_back( e );
        }
    }
}

static bool DFS_Helper( Digraph& dg, ArcFilter& tf, Digraph::Node u, Digraph::Node v, EdgeArray& p )
{
    // 对有向图进行过滤, 过滤掉所有的余枝，然后转换成无向图
    typedef FilterArcs<Digraph, ArcFilter> AFGraph;
    typedef Undirector<AFGraph> Graph;
    typedef Path<Graph> UPath;

    AFGraph afg( dg, tf );
    Graph g( afg );

    // 深度优先搜索(u->v)
    Dfs<Graph> aDfs( g );
    if( !aDfs.run( u, v ) ) return false;

    // 获取路径
    UPath up = aDfs.path( v );
    for( UPath::ArcIt ue( up ); ue != INVALID; ++ue )
    {
        UPath::Arc ua = ue;
        Digraph::Arc a = ua;     // 从无向图的(Arc->Edge)->有向图的Arc
        p.push_back( a );
    }

    return true;
}

VentNetworkCalc::VentNetworkCalc( Digraph& _dg, VNC_EdgeDataMap& _eds, VNC_FanDataMap& _fds )
    : dg( _dg ), ed( _eds ), fd( _fds ), airEdge( INVALID ), speedUp( false )
{
    AddVirtualSTNode( dg, sn, tn );               // 处理多源多汇网络
    airEdge = AddVirtualAirEdge( dg, sn, tn );	 // 增加虚拟大气分支

    InitVirtualSTEdgeData( dg, sn, tn, ed, fd );     // 初始化虚拟源汇分支数据
    InitVirtualAirEdgeData( dg, airEdge, ed, fd );   // 初始化虚拟大气分支数据
}

VentNetworkCalc::~VentNetworkCalc()
{
    DeleteVirtualSTEdgeData( dg, sn, tn, ed );   // 删除虚拟源汇分支数据
    DeleteVirtualAirEdgeData( dg, airEdge, ed ); // 删除虚拟大气分支数据

    DeleteVirtualAirEdge( dg, airEdge );          // 删除虚拟大气分支
    DeleteVirtualSTEdge( dg, sn, tn );            // 删除虚拟源汇分支
    DeleteVirtualSTNode( dg, sn, tn );            // 删除虚拟源汇节点
}

void VentNetworkCalc::setTotalQ( double totalQ )
{
    this->totalQ = totalQ;
    if( totalQ <= 0 ) this->totalQ = DEF_TOTAL_Q;
}

void VentNetworkCalc::setPrecise( double precise )
{
    this->precise = precise;
    if( precise <= 0 || precise >= 1 )
    {
        this->precise = DEF_PRECISE;
    }
}

void VentNetworkCalc::setIterateCount( int maxCount )
{
    this->maxCount = maxCount;
    if( maxCount <= 0 )
    {
        this->maxCount = DEF_MAX_COUNT;
    }
}

bool VentNetworkCalc::iterate()
{
    // 初始化固定风量分支
    // 将固定风量赋给风量
    initFixQEdges();

    speedUp = false;
    bool ret = doIterate( std::min( maxCount, DEF_CONVERGENCE_COUNT ) );

    // 如果迭代不收敛，且迭代次数超过20次，采用加速迭代方法
    if( !ret && maxCount > DEF_CONVERGENCE_COUNT )
    {
        speedUp = true;
        ret = doIterate( maxCount );
    }
    return ret;
}

Digraph::Arc VentNetworkCalc::getAirEdge() const
{
    return airEdge;
}

void VentNetworkCalc::initFixQEdges()
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        double& fq = ed[e]->fq;   // 固定风量
        double& q = ed[e]->q;
        if( fq > 0 )
        {
            q = fq;
        }
    }
}

void VentNetworkCalc::findMST( EdgeArray& tree )
{
    // 初始化权重
    //typedef AFGraph::ArcMap<double> Weight;
    Weight w( dg );
    BuildWeight( dg, ed, fd, speedUp, w );

    // 过滤大气分支
    ArcFilter af( dg, true );
    af[getAirEdge()] = false;

    // 过滤大气分支后的图
    typedef FilterArcs<Digraph, ArcFilter> AFGraph;
    AFGraph afg( dg, af );

    /*
     * 使用kruskal算法求最小生成树
     *
     * 思路：将固定风量分支、风机分支以及的风阻乘以一个较大的系数
     *        使之远大于一般分支的风阻, 则最小生成树的算法不会选择
     *        固定风量分支、风机分支作为树枝
     */
    typedef std::vector<Digraph::Arc> TreeArray;
    TreeArray ta;
    kruskal( afg, w, std::back_inserter( ta ) );

    for( TreeArray::iterator itr = ta.begin(); itr != ta.end(); itr++ )
    {
        tree.push_back( *itr );
    }
}

void VentNetworkCalc::findCycleByTree( EdgeArray& tree, CycleList& cl )
{
    // 过滤余枝(树枝-true, 非树枝-false)
    ArcFilter tf( dg, false );
    BuildTreeFilter( tree, tf );

    // 查找余枝
    EdgeArray leftTree;
    FindLeftTree( dg, tf, leftTree );

    // 对每个余枝搜索回路
    int n = leftTree.size();
    for( int i = 0; i < n; i++ )
    {
        Digraph::Arc e = leftTree[i];          // 回路的基准分支(余枝)
        Digraph::Node u = dg.source( e );      // 基准分支的始点
        Digraph::Node v = dg.target( e );      // 基准分支的末点

        //  查找v->u的一条路径
        EdgeArray p;
        if( !DFS_Helper( dg, tf, v, u, p ) ) continue;

        // 构造独立回路对象
        IndependentCycle* pCycle = new IndependentCycle( dg, ed, fd, airEdge, e );

        // 将路径分支添加到回路对象
        int len = p.size();
        for( int j = 0; j < len; j++ )
        {
            pCycle->addEdge( p[j] );
        }

        // 初始化方向系数
        pCycle->init();

        cl.push_back( pCycle );
    }
}

void VentNetworkCalc::findCycle( CycleList& cl )
{
    // 查找最小生成树
    EdgeArray tree;
    findMST( tree );
    if( tree.empty() ) return;

    // 根据最小生成树查找独立回路
    findCycleByTree( tree, cl );
}

double VentNetworkCalc::sumQ( CycleList& cl, Digraph::Arc e )
{
    double sq = 0;

    int n = cl.size();
    for( int i = 0; i < n; i++ )
    {
        IndependentCycle* pCycle = cl[i];
        double baseQ = ed[pCycle->getBaseEdge()]->q; // 回路基准分支的风量(余枝风量)
        short c = pCycle->getCycleCoeff( e );           // 分支e在回路矩阵中的系数
        sq += c * baseQ;
    }

    return sq;
}

void VentNetworkCalc::initQ( CycleList& cl, double Q )
{
    // 虚拟大气分支的风量=总风量Q
    ed[getAirEdge()]->q = Q;

    // 初始化余枝的风量
    initLeftTreeQ( cl, Q );

    // 计算树枝的风量(实质上也重复的计算了余枝风量)
    initTreeQ( cl );
}

void VentNetworkCalc::initTreeQ( CycleList& cl )
{
    /*
     * 计算树枝的风量(实质上也重复的计算了余枝风量)
     * 采用Q = CT*Qy
     * 其中CT -- 回路矩阵的翻转变换
     *      Qy -- 余枝的风量向量
     * 等价于q = c1*q1 + c2*q2 + ... + ck*qk (其中k=n-m+1)
     */
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        ed[e]->q = sumQ( cl, e );
    }
}

void VentNetworkCalc::initLeftTreeQ( CycleList& cl, double Q )
{
    // 获取余枝，也即回路的基准分支
    EdgeArray leftTree;

    int n = cl.size();
    for( int i = 0; i < n; i++ )
    {
        Digraph::Arc e = cl[i]->getBaseEdge();
        leftTree.push_back( e );
    }

    /*
     * 下面的2个方法都没有考虑"风机分支"的初始风量
     * 目前采用与一般分支相同的处理方式
     * 资料上都说是指定一个"位于高效工况点状态的初始风量"
     * 那么就需要在风机图元中增加一个"初始风量"字段
     * 后续再考虑???
     */
    initLeftTreeQ1( leftTree, Q );
}

void VentNetworkCalc::initLeftTreeQ1( EdgeArray& leftTree, double Q )
{
    /*
     * 大气分支是余枝变量
     * 其它的余枝变量可以任意分配风量
     * 这里采用平均分配的方法
     */
    int n = leftTree.size();
    for( int i = 0; i < n; i++ )
    {
        Digraph::Arc e = leftTree[i];
        if( e == getAirEdge() ) continue; // 排除大气分支
        if( ed[e]->fq > 0 ) continue;    // 排除固定风量分支

        ed[e]->q = Q / n;
    }
}

bool VentNetworkCalc::doIterate( int count )
{
    // 查找独立回路
    CycleList cl;
    findCycle( cl );
    if( cl.empty() ) return true;

    // 初始化风量
    initQ( cl, totalQ );

    // 迭代计算
    bool ret = false;
    int k = 0;  // 迭代次数
    while( !ret && k <= count )
    {
        DValueArray dqArray, dfArray;                    // 记录回路迭代得到的修正风量以及阻力差

        int n = cl.size();
        for( int i = 0; i < n; i++ )
        {
            double dq = 0, df = 0;     // 回路修正风量和阻力差

            cl[i]->iterate( dq, df );  // 每个回路进行迭代

            dqArray.push_back( dq );
            dqArray.push_back( df );
        }

        if( IsMeetErrorRequirement( dqArray, precise ) &&
                IsMeetErrorRequirement( dqArray, precise ) )
        {
            ret = true;
            break;
        }
        else
        {
            k++;
        }
    }

    return ret;
}
