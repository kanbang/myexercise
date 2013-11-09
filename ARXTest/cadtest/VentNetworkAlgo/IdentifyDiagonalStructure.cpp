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

// ͼ����·��
// Digraph dg -> AFGraph dg-> UGraph ug -> UGraph ug

//#include "../Tool/MyRandom.h"
#include "../Tool/Timer.h"

#include "../GraphTool/Tool.h"

// �����ṹ7Ԫ��
// Ŀǰֻ����4��֧�Žڵ�
struct DiagonalStructrue
{
    Digraph::Node s, t;  // �����ڵ㡢�����ڵ�
    Digraph::Node l, r;  // ������㡢�ҹ�����

    DiagonalStructrue() : s( INVALID ), t( INVALID ), l( INVALID ), r( INVALID )
    {
        // Ĭ�ϳ�ʼ��Ϊ��Ч����
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

// ���ڼ�������ķ���������ڵ�
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
        datas[e] = 0; // ��ֹҰָ��
    }
    dg.clear();       // ɾ��ͼ
}

// ���ö����÷��֧
// �����÷�ص㣺�ɾ����桢���������
static bool SetEdgeType( const AcDbObjectId& objId, EdgeData* pEdgeData )
{
    bool ret = true;
    if( objId.isNull() )
    {
        pEdgeData->et = ET_VIRTUAL;
    }
    else if( ArxUtilHelper::IsEqualType( _T( "WorkSurface" ), objId ) )
    {
        pEdgeData->et = ET_USEAIR;  // ������
    }
    else if( ArxUtilHelper::IsEqualType( _T( "StorageGE" ), objId ) )
    {
        if( !ArxUtilHelper::IsEqualType( _T( "MachineRoom" ), objId ) )
        {
            pEdgeData->et = ET_USEAIR;  // ��ҩ�������
        }
        else if( IsPowerSubstation( objId ) )
        {
            pEdgeData->et = ET_USEAIR;  // ��������--�����(�÷�ص�)
        }
    }
    else if( ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId ) )
    {
        pEdgeData->et = ET_USEAIR;  // ���������
    }
    /*else if(IsNeedAir(objId))
    {
    	pEdgeData->et = ET_USEAIR;
    }*/
    return ret;
}

static void InitEdgeDatas( Digraph& dg, ObjectIdMap& om, IDS_EdgeDataMap& datas )
{
    // ������֧����
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        EdgeData* pEdgeData = new EdgeData();
        pEdgeData->objId = om[e];
        SetEdgeType( om[e], pEdgeData );
        datas[e] = pEdgeData; // ����һ��Digraph::Arc����
    }
}

IdentifyDiagonalStructure::IdentifyDiagonalStructure( void )
    : datas( dg ), ef( dg, true ), ngm( dg ), bInitNetwokState( true )
{
    // ��ʼ�������Լ���Ӵ���Դ��
    if( !initNetwok() )
    {
        ClearGraph( dg, datas );
        setInitNetwokState( false );
    }
    else
    {
        // ��ʼ���÷��֧
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
    if( !GetNodeGeoData( dg, om, ngm ) ) return false; // ����Դ��ͻ�������Ϊԭ��(0,0,0)
    FilterBlockEdges( dg, om, ef );

    // ɾ����Ϸ�֧
    EraseBlockEdges( dg, ef );

    // ������֧��������
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
    // Ӧ���жϷ�֧�Ƿ������ͼ��
    // Ŀǰ�ͼ���e�϶���ͼ�ķ�֧
    return datas[e]->objId;
}

// ��s->u·���ϵ����нڵ�
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

    // ��ǽڵ��Ƿ���·��s->u��
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
                nf[ug.source( ue )] = false; // ���·����֧��ʼ�ڵ�
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
    // ֱ��ʹ����ԭʼ����������
    // ���ȷʵӰ�쵽����Ч�ʣ������ٿ����Ż�,ʹ��set�������㷨
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

    // ���˽ڵ�
    UNodeFilter nf( ug, true );

    // ���˽ڵ�������ͼ
    NFUGraph nfug( ug, nf );

    // ��ջ��
    nf[t] = false;
    nf[v] = false;

    Bfs<NFUGraph> aBfs( nfug );
    aBfs.run( u ); // �ӽڵ�s��ʼ��������ͼ
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

// ȷ������ķ���/�����ڵ�
// the nearest source and target
static UGraph::Node FindNST( UGraph& ug,
                             UGraph::Node s, UGraph::Node t,
                             UGraph::Node u, UGraph::Node v )
{
    // s->u������·��֮��Ľڵ�
    NodeArray ns1;
    FindNodesInST( ug, s, t, u, v, ns1 );
    if( ns1.isEmpty() ) return INVALID;

    // s->v������·��֮��Ľڵ�
    NodeArray ns2;
    FindNodesInST( ug, s, t, v, u, ns2 );
    if( ns2.isEmpty() ) return INVALID;

    // ���㽻��
    NodeArray ns;
    GetIntersectNodes( ns1, ns2, ns );

    // ���ؾ���s->u��s->v����Ľڵ�
    return GetMaxDistNodes( ug, s, t, u, v, ns );
}

static bool FindPath( UGraph& ug,
                      UNodeFilter& nf,
                      UGraph::Node u, UGraph::Node v,
                      UPath& p )
{
    // ���˽ڵ�������ͼ
    NFUGraph nfug( ug, nf );

    // ��1��bfs
    Bfs<NFUGraph> aBfs( nfug );

    // ����u->s��·��
    bool ret = aBfs.run( u, v ); // u->s

    // ��ȡ·��p
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
    // ���˽ڵ�
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
            acutPrintf( _T( "\nu->s����ʧ��" ) );
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
            acutPrintf( _T( "\nv->t���ҳɹ�" ) );
            break;
        }
    }

    return ret;
}

static UGraph::Node GetNode( UGraph& ug, UGraph::Arc ue, bool startOrEnd )
{
    return ( startOrEnd ? ug.source( ue ) : ug.target( ue ) );
}

// ��·��p1��p2�Ľ���
// ���bNear=true��������Ľ��㣬��ô���Ƿ�֧��ĩ�ڵ�
// ���bNear=false������Զ�Ľ��㣬��ô���Ƿ�֧��ʼ�ڵ�
static UGraph::Node GetIntersectNode( UGraph& ug, UPath& p1, UPath& p2, bool bNear )
{
    UGraph::Node n = INVALID;

    // Digraph::Node �� UGraph::Node�ǵȼ۵�
    // ʹ��ջ��·�����з�ת
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
        // ���������Զ����
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
        // ��������������
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
    // ת��������ͼ
    UGraph ug( dg );

    // ����������ʹ������ͼ���в���
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        Digraph::Node u = dg.source( e ); // ʼ�ڵ�
        Digraph::Node v = dg.target( e ); // ĩ�ڵ�

        if( u == s || v == s || u == t || v == t ) continue;

        // ȷ������ķ����ڵ�
        //Digraph::Node ns = FindNST(ug, s, t, u, v); // u->s and v->s
        //if(ns == INVALID) continue;

        // ȷ������Ļ����ڵ�
        //Digraph::Node nt = FindNST(ug, t, s, u, v); // u->t and v->t
        //if(nt == INVALID) continue;

        // ����s->u->v->t��·��
        // p1={u->s}, p2={v->t}
        UPath p1, p2;
        if( !FindPath( ug, s, t, u, v, p1, p2 ) ) continue;

        // ����s->v->u->t��·��
        // p3={v->s} p4={u->t}
        UPath p3, p4;
        if( !FindPath( ug, s, t, v, u, p3, p4 ) ) continue;

        // ��·�����з�����ȷ�������ṹ��4��֧�ŵ�
        DiagonalStructrue ds;

        // ����p1��p3ȷ�������ڵ�(�Ƚ�·����֧��ĩ�ڵ�)
        ds.s = GetIntersectNode( ug, p1, p3, true );
        //ds.s = ns;

        // ����p2��p4ȷ�������ڵ�(�Ƚ�·����֧��ĩ�ڵ�)
        ds.t = GetIntersectNode( ug, p2, p4, true );
        //ds.t = nt;

        // ����p1��p4ȷ���������(�Ƚ�·����֧��ʼ�ڵ�)
        ds.l = GetIntersectNode( ug, p1, p4, false );

        // ����p2��p3ȷ���ҹ�����(�Ƚ�·����֧��ʼ�ڵ�)
        ds.r = GetIntersectNode( ug, p2, p3, false );

        // �ý����ṹ����Ч��
        if( ds.isValid() )
        {
            // ��ӵ������ṹ������
            int pos = dss.find( ds );
            if( pos == -1 ) pos = dss.append( ds ); // pos��¼��ӵ�Ԫ�ص�����λ��
            ds_map[e] = pos;
        }
    }
}

static void PrintDS( DiagonalStructrue& ds, NodeGeoDataMap& ngm )
{
    AcGePoint3d pt = ngm[ds.s];
    acutPrintf( _T( "\n����������:(%.3f, %.3f, %.3f)" ), pt.x, pt.y, pt.z );

    pt = ngm[ds.t];
    acutPrintf( _T( "\n����������:(%.3f, %.3f, %.3f)" ), pt.x, pt.y, pt.z );

    pt = ngm[ds.l];
    acutPrintf( _T( "\n�����������:(%.3f, %.3f, %.3f)" ), pt.x, pt.y, pt.z );

    pt = ngm[ds.r];
    acutPrintf( _T( "\n�ҹ���������:(%.3f, %.3f, %.3f)" ), pt.x, pt.y, pt.z );
}

static bool FindDS( Digraph& dg,
                    NodeGeoDataMap& ngm,
                    Digraph::Node s, Digraph::Node t,
                    Digraph::Arc e, DiagonalStructrue& ds )
{
    Digraph::Node u = dg.source( e ); // ʼ�ڵ�
    Digraph::Node v = dg.target( e ); // ĩ�ڵ�

    if( u == s || v == s || u == t || v == t ) return false;

    // ת��������ͼ
    UGraph ug( dg );

    // ȷ������ķ����ڵ�
    Digraph::Node ns = FindNST( ug, s, t, u, v ); // u->s and v->s
    if( ns == INVALID )
    {
        acutPrintf( _T( "\n�޷��ҵ������ڵ�..." ) );
        return false;
    }

    // ȷ������Ļ����ڵ�
    //Digraph::Node nt = FindNST(ug, t, s, u, v); // u->t and v->t
    Digraph::Node nt = FindNST( ug, t, ns, u, v ); // u->t and v->t
    if( nt == INVALID )
    {
        acutPrintf( _T( "\n�޷��ҵ������ڵ�..." ) );
        return false;
    }

    if( ns == nt )
    {
        acutPrintf( _T( "\n��������!!!���ҵ��ķ����ڵ�ͻ����ڵ���ͬ!!!" ) );
        return false;
    }

    // ����s->u->v->t��·��
    // p1={u->s}, p2={v->t}
    UPath p1, p2;
    if( !FindPath( ug, ns, nt, u, v, p1, p2 ) )
    {
        acutPrintf( _T( "\n����·��s->u->v->tʧ��..." ) );
        return false;
    }

    // ����s->v->u->t��·��
    // p3={v->s} p4={u->t}
    UPath p3, p4;
    if( !FindPath( ug, ns, nt, v, u, p3, p4 ) )
    {
        acutPrintf( _T( "\n����·��s->v->u->tʧ��..." ) );
        return false;
    }

    // ��·�����з�����ȷ�������ṹ��4��֧�ŵ�

    // ����p1��p3ȷ�������ڵ�(�Ƚ�·����֧��ĩ�ڵ�)
    ds.s = GetIntersectNode( ug, p1, p3, true );

    // ����p2��p4ȷ�������ڵ�(�Ƚ�·����֧��ĩ�ڵ�)
    ds.t = GetIntersectNode( ug, p2, p4, true );

    // ����p1��p4ȷ���������(�Ƚ�·����֧��ʼ�ڵ�)
    ds.l = GetIntersectNode( ug, p1, p4, false );

    // ����p2��p3ȷ���ҹ�����(�Ƚ�·����֧��ʼ�ڵ�)
    ds.r = GetIntersectNode( ug, p2, p3, false );

    PrintDS( ds, ngm );

    // �ý����ṹ����Ч��
    return ds.isValid();
}

// ������ͼ��֧ת��������ͼ��֧
static Digraph::Arc UArc2Arc( UGraph::Arc ue )
{
    Digraph::Arc e = ue;
    return e;
}

static bool GetDS( Digraph::Arc e, DSMap& ds_map, DSArray& dss, DiagonalStructrue& ds )
{
    // �������-1����÷�֧���ǽ�����֧
    int pos = ds_map[e];
    if( pos == -1 ) return false;

    // ��ȡ�����ṹ
    ds = dss[pos];

    return true;
}

static bool FindDSPath( Digraph& dg, DiagonalStructrue& ds, EdgeArray& pp1, EdgeArray& pp2 )
{
    // ת��������ͼ
    UGraph ug( dg );

    // �������ҹ���ͨ·
    UPath p1, p2;
    if( !FindPath( ug, ds.s, ds.t, ds.l, ds.r, p1, p2 ) ) return false;

    UPath p3, p4;
    if( !FindPath( ug, ds.s, ds.t, ds.r, ds.l, p3, p4 ) ) return false;

    // p1��p4�ϲ�Ϊ�����ͨ·
    for( UPath::ArcIt ue( p1 ); ue != INVALID; ++ue )
    {
        pp1.append( UArc2Arc( ue ) );
    }
    for( UPath::ArcIt ue( p2 ); ue != INVALID; ++ue )
    {
        pp1.append( UArc2Arc( ue ) );
    }

    // p2��p3�ϲ�Ϊ�ҹ���ͨ·
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
    // ��¼����֧����Ӧ�Ľ����ṹ
    DSMap ds_map( dg );
    for( Digraph::ArcIt ee( dg ); ee != INVALID; ++ee )
    {
        ds_map[ee] = -1; // Ĭ��Ϊ��Ч����
    }

    // ���ʹ��UGraph::ArcIt��������ͼ�ķ�֧
    // ÿ����֧���ᱻ����2��(����ͷ���)

    // �����ṹ����
    DSArray dss;

    // �������еĽ����ṹ
    FindAllDS( dg, sn, tn, ds_map, dss );

    // ��ȡ�����ṹ
    DiagonalStructrue ds;
    if( !GetDS( e, ds_map, dss, ds ) ) return false;

    return FindDSPath( dg, ds, pp1, pp2 );
}

static bool IDS_Helper( Digraph& dg, Digraph::Node sn, Digraph::Node tn, NodeGeoDataMap& ngm, Digraph::Arc e, EdgeArray& pp1, EdgeArray& pp2, bool findNST = false )
{
    // ���ҷ�֧��Ӧ�Ľ����ṹ
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
    acutPrintf( _T( "\n����������:(%.3f, %.3f, %.3f)" ), pt.x, pt.y, pt.z );

    pt = ngm[ds.t];
    acutPrintf( _T( "\n����������:(%.3f, %.3f, %.3f)" ), pt.x, pt.y, pt.z );

    return FindDSPath( dg, ds, pp1, pp2 );
}

bool IdentifyDiagonalStructure::isDS( const AcDbObjectId& objId )
{
    if( !isInitNetworkOk() ) return false;

    Digraph::Arc e = getEdgeById( objId );
    if( e == INVALID ) return false;
    if( !ef[e] ) return false; // ��Ϸ�֧

    Digraph::Node u = dg.source( e );
    Digraph::Node v = dg.target( e );

    UGraph ug( dg );

    // �������ҹ���ͨ·
    UPath p1, p2;
    if( !FindPath( ug, sn, tn, u, v, p1, p2 ) )
    {
        acutPrintf( _T( "\n����·��s->u->v->tʧ��..." ) );
        return false;
    }

    UPath p3, p4;
    if( !FindPath( ug, sn, tn, v, u, p3, p4 ) )
    {
        acutPrintf( _T( "\n����·��s->v->u->tʧ��..." ) );
        return false;
    }

    return true;
}

// findNST -- �Ƿ��������ķ����ڵ�ͻ����ڵ�
bool IdentifyDiagonalStructure::doIDS( const AcDbObjectId& objId, AcDbObjectIdArray& p1, AcDbObjectIdArray& p2, bool findNST )
{
    if( !isInitNetworkOk() ) return false;

    Digraph::Arc e = getEdgeById( objId );
    if( e == INVALID )
    {
        acutPrintf( _T( "\n�÷�֧������" ) );
        return false;
    }

    acutPrintf( _T( "\n��ʼʶ������ṹ..." ) );
    EdgeArray pp1, pp2;
    if( !IDS_Helper( dg, sn, tn, ngm, e, pp1, pp2, findNST ) ) return false;

    acutPrintf( _T( "\n��ȡ���ҹ���ͨ·��ʵ��id..." ) );
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

    // ���ҽ����ṹ��Ӧ�����з�֧
    return true;
}
