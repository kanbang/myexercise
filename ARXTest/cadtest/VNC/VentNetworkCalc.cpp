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
    NodeArray sNodes;  // Դ�㼯��

    // ��������з���,����Դ��
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        // ��¼���=0��Դ��
        if( 0 == countInArcs( dg, n ) ) sNodes.push_back( n );
    }

    /*
     * !!!ǰ������!!!
     * ������һ��Դ���һ�����
     * Ŀǰ��ʱ������������"���������"
     * ���磺û��Դ���û�л�㣬����Դ��㶼û��
     */
    if( sNodes.empty() ) return INVALID;

    // ����һ������Դ��
    Digraph::Node sn = dg.addNode();

    // ����Դ��"������֧"(����Դ��-->Դ��)
    int len = sNodes.size();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = dg.addArc( sn, sNodes[i] );
    }
    return sn;  // ��������������Դ��
}

static Digraph::Node AddVirtualTarget( Digraph& dg )
{
    NodeArray tNodes;  // Դ�㼯��

    // ��������з���,���һ��
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        // ��¼����=0�Ļ��
        if( 0 == countOutArcs( dg, n ) ) tNodes.push_back( n );
    }

    /*
     * !!!ǰ������!!!
     * ������һ��Դ���һ�����
     * Ŀǰ��ʱ������������"���������"
     * ���磺û��Դ���û�л�㣬����Դ��㶼û��
     */

    if( tNodes.empty() ) return INVALID;

    // ����һ��������
    Digraph::Node tn = dg.addNode();

    // ���ӻ��"�����֧"(���-->������)
    int len = tNodes.size();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = dg.addArc( tNodes[i], tn );
    }
    return tn; // ����������������
}

static bool AddVirtualSTNode( Digraph& dg, Digraph::Node& sn, Digraph::Node& tn )
{
    // ����һ������Դ�㼰��֧
    /*Digraph::Node*/ sn = AddVirtualSource( dg );

    // ����һ�������㼰��֧
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
    // ɾ�������֧
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
    // ɾ������Դ��ڵ�
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
    // �����ֵ����Ԫ��
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
    // �������ݵľ���ֵ�ľ�����
    int n = dvalues.size();

    // 1��ƽ��ֵ
    double a = 0;
    for( int i = 0; i < n; i++ )
    {
        a += abs( dvalues[i] );
    }
    a = a / n;

    // 2��������
    double s = 0;
    for( int i = 0; i < n; i++ )
    {
        double v = abs( dvalues[i] );
        s += ( a - v ) * ( a - v );
    }
    return sqrt( s / n );
}

// Ҳ���Բο�"����ͳ��"�е�һЩ�����㷽��
static bool IsMeetErrorRequirement( DValueArray& dvalues, double precise )
{
    if( dvalues.empty() ) return true;

    double maxD = MaxAbsValue( dvalues );

    bool ret = ( maxD - precise ) <= 0;

    //acutPrintf(_T("\n���ƫ��ֵ:%.5f, %s����Ҫ��:%.5f\n"), maxD, (ret?_T("����"):_T("������")), precise);

    // �Ƚ�"�������ݵľ���ֵ"��"����"�Ĵ�С
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
    double fq = ed[e]->fq; // �̶�����

    // �Ŵ�̶�������֧�ͷ����֧(��Ϊ��֦)
    double c = abs( speedUp ? r * q : r );

    if( fd[e] != 0 )
    {
        c = Magnify_Double( c, LARGE_COEFF );  // �����֧
    }

    if( fq > 0 )
    {
        c = Magnify_Double( c, LARGE_COEFF ); // �̶�������֧
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

// Tree Filter == > tfĬ��ȫ��Ϊfalse
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
    // ������ͼ���й���, ���˵����е���֦��Ȼ��ת��������ͼ
    typedef FilterArcs<Digraph, ArcFilter> AFGraph;
    typedef Undirector<AFGraph> Graph;
    typedef Path<Graph> UPath;

    AFGraph afg( dg, tf );
    Graph g( afg );

    // �����������(u->v)
    Dfs<Graph> aDfs( g );
    if( !aDfs.run( u, v ) ) return false;

    // ��ȡ·��
    UPath up = aDfs.path( v );
    for( UPath::ArcIt ue( up ); ue != INVALID; ++ue )
    {
        UPath::Arc ua = ue;
        Digraph::Arc a = ua;     // ������ͼ��(Arc->Edge)->����ͼ��Arc
        p.push_back( a );
    }

    return true;
}

VentNetworkCalc::VentNetworkCalc( Digraph& _dg, VNC_EdgeDataMap& _eds, VNC_FanDataMap& _fds )
    : dg( _dg ), ed( _eds ), fd( _fds ), airEdge( INVALID ), speedUp( false )
{
    AddVirtualSTNode( dg, sn, tn );               // �����Դ�������
    airEdge = AddVirtualAirEdge( dg, sn, tn );	 // �������������֧

    InitVirtualSTEdgeData( dg, sn, tn, ed, fd );     // ��ʼ������Դ���֧����
    InitVirtualAirEdgeData( dg, airEdge, ed, fd );   // ��ʼ�����������֧����
}

VentNetworkCalc::~VentNetworkCalc()
{
    DeleteVirtualSTEdgeData( dg, sn, tn, ed );   // ɾ������Դ���֧����
    DeleteVirtualAirEdgeData( dg, airEdge, ed ); // ɾ�����������֧����

    DeleteVirtualAirEdge( dg, airEdge );          // ɾ�����������֧
    DeleteVirtualSTEdge( dg, sn, tn );            // ɾ������Դ���֧
    DeleteVirtualSTNode( dg, sn, tn );            // ɾ������Դ��ڵ�
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
    // ��ʼ���̶�������֧
    // ���̶�������������
    initFixQEdges();

    speedUp = false;
    bool ret = doIterate( std::min( maxCount, DEF_CONVERGENCE_COUNT ) );

    // ����������������ҵ�����������20�Σ����ü��ٵ�������
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
        double& fq = ed[e]->fq;   // �̶�����
        double& q = ed[e]->q;
        if( fq > 0 )
        {
            q = fq;
        }
    }
}

void VentNetworkCalc::findMST( EdgeArray& tree )
{
    // ��ʼ��Ȩ��
    //typedef AFGraph::ArcMap<double> Weight;
    Weight w( dg );
    BuildWeight( dg, ed, fd, speedUp, w );

    // ���˴�����֧
    ArcFilter af( dg, true );
    af[getAirEdge()] = false;

    // ���˴�����֧���ͼ
    typedef FilterArcs<Digraph, ArcFilter> AFGraph;
    AFGraph afg( dg, af );

    /*
     * ʹ��kruskal�㷨����С������
     *
     * ˼·�����̶�������֧�������֧�Լ��ķ������һ���ϴ��ϵ��
     *        ʹ֮Զ����һ���֧�ķ���, ����С���������㷨����ѡ��
     *        �̶�������֧�������֧��Ϊ��֦
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
    // ������֦(��֦-true, ����֦-false)
    ArcFilter tf( dg, false );
    BuildTreeFilter( tree, tf );

    // ������֦
    EdgeArray leftTree;
    FindLeftTree( dg, tf, leftTree );

    // ��ÿ����֦������·
    int n = leftTree.size();
    for( int i = 0; i < n; i++ )
    {
        Digraph::Arc e = leftTree[i];          // ��·�Ļ�׼��֧(��֦)
        Digraph::Node u = dg.source( e );      // ��׼��֧��ʼ��
        Digraph::Node v = dg.target( e );      // ��׼��֧��ĩ��

        //  ����v->u��һ��·��
        EdgeArray p;
        if( !DFS_Helper( dg, tf, v, u, p ) ) continue;

        // ���������·����
        IndependentCycle* pCycle = new IndependentCycle( dg, ed, fd, airEdge, e );

        // ��·����֧��ӵ���·����
        int len = p.size();
        for( int j = 0; j < len; j++ )
        {
            pCycle->addEdge( p[j] );
        }

        // ��ʼ������ϵ��
        pCycle->init();

        cl.push_back( pCycle );
    }
}

void VentNetworkCalc::findCycle( CycleList& cl )
{
    // ������С������
    EdgeArray tree;
    findMST( tree );
    if( tree.empty() ) return;

    // ������С���������Ҷ�����·
    findCycleByTree( tree, cl );
}

double VentNetworkCalc::sumQ( CycleList& cl, Digraph::Arc e )
{
    double sq = 0;

    int n = cl.size();
    for( int i = 0; i < n; i++ )
    {
        IndependentCycle* pCycle = cl[i];
        double baseQ = ed[pCycle->getBaseEdge()]->q; // ��·��׼��֧�ķ���(��֦����)
        short c = pCycle->getCycleCoeff( e );           // ��֧e�ڻ�·�����е�ϵ��
        sq += c * baseQ;
    }

    return sq;
}

void VentNetworkCalc::initQ( CycleList& cl, double Q )
{
    // ���������֧�ķ���=�ܷ���Q
    ed[getAirEdge()]->q = Q;

    // ��ʼ����֦�ķ���
    initLeftTreeQ( cl, Q );

    // ������֦�ķ���(ʵ����Ҳ�ظ��ļ�������֦����)
    initTreeQ( cl );
}

void VentNetworkCalc::initTreeQ( CycleList& cl )
{
    /*
     * ������֦�ķ���(ʵ����Ҳ�ظ��ļ�������֦����)
     * ����Q = CT*Qy
     * ����CT -- ��·����ķ�ת�任
     *      Qy -- ��֦�ķ�������
     * �ȼ���q = c1*q1 + c2*q2 + ... + ck*qk (����k=n-m+1)
     */
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        ed[e]->q = sumQ( cl, e );
    }
}

void VentNetworkCalc::initLeftTreeQ( CycleList& cl, double Q )
{
    // ��ȡ��֦��Ҳ����·�Ļ�׼��֧
    EdgeArray leftTree;

    int n = cl.size();
    for( int i = 0; i < n; i++ )
    {
        Digraph::Arc e = cl[i]->getBaseEdge();
        leftTree.push_back( e );
    }

    /*
     * �����2��������û�п���"�����֧"�ĳ�ʼ����
     * Ŀǰ������һ���֧��ͬ�Ĵ���ʽ
     * �����϶�˵��ָ��һ��"λ�ڸ�Ч������״̬�ĳ�ʼ����"
     * ��ô����Ҫ�ڷ��ͼԪ������һ��"��ʼ����"�ֶ�
     * �����ٿ���???
     */
    initLeftTreeQ1( leftTree, Q );
}

void VentNetworkCalc::initLeftTreeQ1( EdgeArray& leftTree, double Q )
{
    /*
     * ������֧����֦����
     * ��������֦������������������
     * �������ƽ������ķ���
     */
    int n = leftTree.size();
    for( int i = 0; i < n; i++ )
    {
        Digraph::Arc e = leftTree[i];
        if( e == getAirEdge() ) continue; // �ų�������֧
        if( ed[e]->fq > 0 ) continue;    // �ų��̶�������֧

        ed[e]->q = Q / n;
    }
}

bool VentNetworkCalc::doIterate( int count )
{
    // ���Ҷ�����·
    CycleList cl;
    findCycle( cl );
    if( cl.empty() ) return true;

    // ��ʼ������
    initQ( cl, totalQ );

    // ��������
    bool ret = false;
    int k = 0;  // ��������
    while( !ret && k <= count )
    {
        DValueArray dqArray, dfArray;                    // ��¼��·�����õ������������Լ�������

        int n = cl.size();
        for( int i = 0; i < n; i++ )
        {
            double dq = 0, df = 0;     // ��·����������������

            cl[i]->iterate( dq, df );  // ÿ����·���е���

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
