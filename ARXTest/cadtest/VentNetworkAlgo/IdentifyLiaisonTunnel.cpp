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

// ����÷�ص��ʼĩ�ڵ������Դ��ڵ�
// ��������ʱ�򣬲�������Щ�ڵ�
static void BuildNodeFilter( Digraph& dg, EdgeArray& airEdges, Digraph::Node s, Digraph::Node t, NodeFilter& nf )
{
    int len = airEdges.length();
    acutPrintf( _T( "\n�÷�ص����:%d" ), len );
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = airEdges[i];
        nf[dg.source( e )] = false;
        nf[dg.target( e )] = false;
    }

    nf[s] = false;
    nf[t] = false;

    // ԭʼ�����Դ��ͻ��ҲҪ���
    for( Digraph::OutArcIt e( dg, s ); e != INVALID; ++e )
    {
        nf[dg.target( e )] = false;
    }
    for( Digraph::InArcIt e( dg, t ); e != INVALID; ++e )
    {
        nf[dg.source( e )] = false;
    }
}

// ʹ�ÿ����������BFS������̾���
static void SortAirEdges( Digraph& dg, ArcFilter& ef, Digraph::Node s, Digraph::Node t, const EdgeArray& airEdges, EdgeArray& es )
{
    AFGraph afg( dg, ef );

    Bfs<AFGraph> aBfs( afg );
    aBfs.run( s );
    //if(!aDfs.reached(t)) return; // s->t���ɴ�

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
            acutPrintf( _T( "\n��%d���÷�ص㡾ʼ�㡿���ɴ�" ), i );
        }

        if( bBfs.reached( v ) )
        {
            aed.td = bBfs.dist( v );
        }
        else
        {
            acutPrintf( _T( "\n��%d���÷�ص㡾ĩ�㡿���ɴ�" ), i );
        }

        aes.push_back( aed );
    }

    // ��������
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
        imap[n] = ( nf[n] ? 1 : 0 ); // Ĭ�����нڵ�λ�ڽ�����
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
            if( nf[n] && bDfs.reached( n ) ) imap[n] = -1; // �ڵ�λ�ڻط���
        }
    }
}

static void FilterILTEdge( Digraph& dg, IntMap& imap, ArcFilter& ef )
{
    // ����������
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        Digraph::Node u = dg.source( e );
        Digraph::Node v = dg.target( e );

        // �ų�ֱ�������ķ�֧
        // �������֧��ʼ�ڵ���ȱ������1��ĩ�ڵ����ȱ������1
        if( countOutArcs( dg, u ) <= 1 || countInArcs( dg, v ) <= 1 ) continue;

        if( imap[u]*imap[v] < 0 ) ef[e] = false;
    }
}

// cmap��ʾ�����÷�ص�Ĵ���
// �������0�����ʾ����÷�ص�Ľ���·�߾����ýڵ�
// ���С��0�����ʾ����÷�ص�Ļط�·�߾����ýڵ�
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
    // ����ͼ
    RDigraph rdg( afg );

    int minCount = 1000;
    Dfs<RDigraph> aDfs( rdg );
    aDfs.run( u ); // dfs�������нڵ�
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
    // ����ͼ
    RDigraph rdg( afg );

    Dfs<RDigraph> aDfs( rdg );
    aDfs.run( u ); // dfs�������нڵ�
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

    // ����dfs
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

        // �ų�ֱ�������ķ�֧
        // �������֧��ʼ�ڵ���ȱ������1��ĩ�ڵ����ȱ������1
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
    // ��¼�÷�ص��ʼĩ�����õ���count
    AcDbIntArray sAirs, tAirs;

    double st = Timer::rdtscSeconds();

    // ��ʾ������÷�ص��˳����
    //DisplayAirEdgeNum(sortedAirEdges, datas);

    int len = sortedAirEdges.length();
    acutPrintf( _T( "\n�÷�ص����:%d" ), len );
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = sortedAirEdges[i];
        Digraph::Node u = dg.source( e );
        Digraph::Node v = dg.target( e );

        sAirs.append( CountInFlowNodes( dg, ef, nf, u, cmap ) );
        tAirs.append( CountOutFlowNodes( dg, ef, nf, v, cmap ) );
        //acutPrintf(_T("\n��%d���÷�ص㡾ʼ�ڵ㡿=%d  ��ĩ�ڵ㡿=%d"), i, sAirs[i], tAirs[i]);
    }

    /*for(Digraph::NodeIt n(dg); n!=INVALID; ++n)
    {
    	if(nf[n] && imap[n]==0) imap[n] = 1;
    }*/

    double et = Timer::rdtscSeconds();
    acutPrintf( _T( "\n��1������dfs���Һķ�ʱ��:%.5f" ), et - st );

    // ��ʾ��֧��ʼĩ�ڵ��cmapֵ(������)
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
    acutPrintf( _T( "\n��2������dfs���Һķ�ʱ��:%.5f" ), et - st );

    /*
     * ������֮�󣬿��ܳ���һЩ��֧��ʼ�ڵ�(��ĩ�ڵ�)��cmap=0
     * ��ĩ�ڵ�(��ʼ�ڵ�)��cmap������0
     * ��Щ��֧�п����в��ַ�֧Ҳ����Ϊ��Ҫ���ط�֮���������
     */
    // ��ʾ��֧��ʼĩ�ڵ��cmapֵ(������)
    //DisplayEdgeCMapValue(dg, sortedAirEdges, datas, cmap, sAirs, tAirs);
}

static void ClearGraph( Digraph& dg, ILT_EdgeDataMap2& datas )
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

static void InitEdgeDatas( Digraph& dg, ObjectIdMap& om, ILT_EdgeDataMap2& datas )
{
    // ������֧����
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        EdgeData* pEdgeData = new EdgeData();
        pEdgeData->objId = om[e];
        SetEdgeType( pEdgeData->objId, pEdgeData );
        datas[e] = pEdgeData; // ����һ��Digraph::Arc����
    }
}

IdentifyLiaisonTunnel::IdentifyLiaisonTunnel( void ) : datas( dg ), ef( dg, true ), bInitNetwokState( true )
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

    // ������֧��������
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
    // Ӧ���жϷ�֧�Ƿ������ͼ��
    // Ŀǰ�ͼ���e�϶���ͼ�ķ�֧
    return datas[e]->objId;
}

bool IdentifyLiaisonTunnel::doILT2( EdgeArray& edges )
{
    if( !isInitNetworkOk() ) return false;
    if( airEdges.isEmpty() ) return false;

    NodeFilter nf( dg, true );
    BuildNodeFilter( dg, airEdges, sn, tn, nf );

    // ��¼�ڵ������ĸ�����(0-�÷���, 1-������, -1-�ط���)
    IntMap imap( dg );
    Init_IMap( dg, ef, nf, airEdges, imap );

    // ����������
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
    acutPrintf( _T( "\n������Ϸ�֧�ķ�ʱ��:%.5f" ), et - st );

    IntMap cmap( dg );
    Init_CMap( dg, cmap );

    st = Timer::rdtscSeconds();

    // ���÷�ص㰴�����������
    EdgeArray sortedAirEdges;
    SortAirEdges( dg, ef, sn, tn, airEdges, sortedAirEdges );

    et = Timer::rdtscSeconds();
    acutPrintf( _T( "\n���÷�ص㰴��������ķ�ʱ��:%.5f" ), et - st );

    ILT_Helper( dg, datas, ef, nf, sortedAirEdges, cmap );

    st = Timer::rdtscSeconds();

    // ����������
    FindILTEdges( dg, datas, cmap, edges );

    et = Timer::rdtscSeconds();
    acutPrintf( _T( "\n����������ķ�ʱ��:%.5f" ), et - st );

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

    // ��¼�ڵ����Ⱥͳ���
    // ������Ϸ�֧
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
                // �ط���,����ڵ������Ƿ����·�����
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
        if( datas[e]->et == ET_VIRTUAL ) continue; // �ų�������������ʱ���ӵ������֧
        es1.append( e );
    }
    for( Digraph::OutArcIt e( dg, dg.target( le ) ); e != INVALID; ++e )
    {
        if( datas[e]->et == ET_VIRTUAL ) continue; // �ų�������������ʱ���ӵ������֧
        es2.append( e );
    }
    return true;
}