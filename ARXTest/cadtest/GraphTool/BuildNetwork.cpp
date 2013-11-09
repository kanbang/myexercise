#include "stdafx.h"
#include "BuildNetwork.h"

#include "../MineGE/TagGE.h"
#include "../MineGE/LinkedGE.h"
#include "../DefGE/TTunnel.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

//#include <set>
//#include <stack>
#include <vector>
#include <algorithm>

// ����ʱ��
//#include "../Tool/MyRandom.h"
#include "../Tool/Timer.h"

static void FindBlocks( AcDbObjectIdArray& tagObjIds )
{
    DrawHelper::FindMineGEs( _T( "Wall" ), tagObjIds );
    DrawHelper::FindMineGEs( _T( "DoubleGate" ), tagObjIds );
    DrawHelper::FindMineGEs( _T( "BalanceGate" ), tagObjIds );
    //DrawHelper::FindMineGEs(_T("Gate"), tagObjIds);
}

// ���ҷ�֧ͼԪ������������桢���ҵ�
static void FindEdgeGEs( AcDbObjectIdArray& objIds )
{
    DrawHelper::FindMineGEs( _T( "LinkedGE" ), objIds );
}

static void GetEdgeSEPts( const AcDbObjectIdArray& objIds, AcGePoint3dArray& spts, AcGePoint3dArray& epts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;
        LinkedGE* pEdge = LinkedGE::cast( pObj );

        AcGePoint3d spt, ept;
        pEdge->getSEPoint( spt, ept );

        // ��¼ʼĩ������
        spts.append( spt );
        epts.append( ept );
    }
    actrTransactionManager->endTransaction();
}

static void GetNodePts( const AcGePoint3dArray& spts, const AcGePoint3dArray& epts, AcGePoint3dArray& pts )
{
    int len = spts.length();
    for( int i = 0; i < len; i++ )
    {
        if( !pts.contains( spts[i] ) ) pts.append( spts[i] );
        if( !pts.contains( epts[i] ) ) pts.append( epts[i] );
    }
}

static void BuildNodesMap( const AcGePoint3dArray& pts, Digraph& dg, NodeArray& nodes )
{
    // ����������ͽڵ�֮���ӳ��
    int len = pts.length();
    for( int i = 0; i < len; i++ )
    {
        nodes.append( dg.addNode() );
    }
}

static void BuildEdgesMap( const AcDbObjectIdArray& objIds, const AcGePoint3dArray& pts, Digraph& dg, NodeArray& nodes, EdgeArray& edges )
{
    // ��ӷ�֧
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;
        LinkedGE* pEdge = LinkedGE::cast( pObj );

        AcGePoint3d startPt, endPt;
        pEdge->getSEPoint( startPt, endPt );
        Digraph::Node u = nodes[pts.find( startPt )];
        Digraph::Node v = nodes[pts.find( endPt )];
        edges.append( dg.addArc( u, v ) );
    }
    actrTransactionManager->endTransaction();
}

static void EraseZeroDegreeNodes( Digraph& dg )
{
    // ���Ҳ�ɾ����(����+���)����0�Ľڵ�
    NodeArray ns;
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        if( ( countOutArcs( dg, n ) + countInArcs( dg, n ) ) == 0 )
        {
            ns.append( n );
        }
    }
    int len = ns.length();
    for( int i = 0; i < len; i++ )
    {
        dg.erase( ns[i] ); // ɾ���սڵ�(�ȵ���0)
    }
}

// ��ȡ���������ĵ������
static bool GetInTunnel( const AcDbObjectId& objId, AcDbObjectId& inTunnel )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    TTunnel* pTT = TTunnel::cast( pObj );
    if( pTT == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    inTunnel = pTT->getInTunnel();

    actrTransactionManager->endTransaction();

    return !inTunnel.isNull();
}

// ��objIds�в��Ҿ�������棬ͬʱ���Ҿ���������Ӧ�ĵ������
static void FindTTunnels( const AcDbObjectIdArray& objIds, AcDbObjectIdArray& tObjIds, AcDbObjectIdArray& inTunnels )
{
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        if( !ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objIds[i] ) ) continue;

        AcDbObjectId inTunnel;
        if( !GetInTunnel( objIds[i], inTunnel ) ) continue;

        tObjIds.append( objIds[i] );
        inTunnels.append( inTunnel );
    }
}

// ����֧es��Դ����v
static void ChangeOutArcSourceNode( Digraph& dg, EdgeArray& es, Digraph::Node v )
{
    int len = es.length();
    for( int i = 0; i < len; i++ )
    {
        dg.changeSource( es[i], v );
    }
}

// ����֧es��ĩ����v
static void ChangeInArcTargetNode( Digraph& dg, EdgeArray& es, Digraph::Node v )
{
    int len = es.length();
    for( int i = 0; i < len; i++ )
    {
        dg.changeTarget( es[i], v );
    }
}

// ���ѵ��������ĩ�ڵ�, �ں���������һ������ڵ�
// �÷������޸�objIds��edges
static void SplitTargetNode( AcDbObjectIdArray& objIds, Digraph& dg, EdgeArray& edges, const EdgeArray& es )
{
    // ��¼���������֧�����й�ͬ��ĩ�ڵ�(�ų��ظ����)
    NodeArray ns;
    int len = es.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Node u = dg.target( es[i] );
        if( !ns.contains( u ) ) ns.append( u );
    }

    // �ڵ������ĩ�ڵ����������һ������ڵ�
    len = ns.length();
    acutPrintf( _T( "\n������������%d������ڵ�" ), len );
    for( int i = 0; i < len; i++ )
    {
        Digraph::Node u = ns[i];
        Digraph::Node v = dg.addNode(); // ����һ������ڵ�

        EdgeArray outEs;
        for( Digraph::OutArcIt e( dg, u ); e != INVALID; ++e )
        {
            if( !es.contains( e ) ) outEs.append( e );
        }

        EdgeArray inEs;
        for( Digraph::InArcIt e( dg, u ); e != INVALID; ++e )
        {
            if( !es.contains( e ) ) inEs.append( e );
        }

        // �޸Ľڵ�u�����г��߷�֧(���������������֧)��ʼ������
        ChangeOutArcSourceNode( dg, outEs, v );

        // �޸Ľڵ�u��������߷�֧(���������������֧)��ĩ������
        ChangeInArcTargetNode( dg, inEs, v );

        // ���������֧,��ӦidΪnull
        objIds.append( AcDbObjectId::kNull );
        edges.append( dg.addArc( u, v ) );
    }
}

static void SplitInTunnelNode( AcDbObjectIdArray& objIds, AcDbObjectIdArray& tObjIds, AcDbObjectIdArray& inTunnels, Digraph& dg, EdgeArray& edges )
{
    EdgeArray es;
    int len = tObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        int pos = objIds.find( inTunnels[i] );
        es.append( edges[pos] );
    }
    SplitTargetNode( objIds, dg, edges, es );
}

// �����������ĩ�ڵ�����ӵ�����ڵ���������
static void LinkInTunnelToTTunnel( AcDbObjectIdArray& objIds, AcDbObjectIdArray& tObjIds, AcDbObjectIdArray& inTunnels, Digraph& dg, EdgeArray& edges )
{
    int len = tObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        int p1 = objIds.find( inTunnels[i] );
        int p2 = objIds.find( tObjIds[i] );

        // "�������"��ĩ�ڵ������������֧��ʼ�ڵ�
        // ֮������һ�������֧(�÷�֧�ɿ����Ƿ�Ͳ)
        Digraph::Node u = dg.target( edges[p1] ); // �������ĩ�ڵ�
        Digraph::Node v = dg.source( edges[p2] ); // ���������ʼ�ڵ�

        edges.append( dg.addArc( u, v ) );
        objIds.append( AcDbObjectId::kNull );     // �������"��Ͳ"��id���ܲ�Ϊ��???
    }
}

// ����������뵼�����֮�����������֧
static void DealWithTTunnel( AcDbObjectIdArray& objIds, Digraph& dg, EdgeArray& edges )
{
    // ���Ҿ���������Լ���Ӧ�ĵ������(��Ͳ�������)
    AcDbObjectIdArray tObjIds;
    AcDbObjectIdArray inTunnels;
    FindTTunnels( objIds, tObjIds, inTunnels );

    //assert(tObjIds.length() == inTunnels.length());

    //double st = Timer::rdtscSeconds();

    // ��������ָ��2��(���һ������ڵ㣬�Լ�һ�������֧)
    SplitInTunnelNode( objIds, tObjIds, inTunnels, dg, edges );

    //double et = Timer::rdtscSeconds();
    //acutPrintf(_T("\n��������������ڵ㣬�ķ�ʱ��:%.5f,������������:%d,ƽ��ʱ��:%.5f"), et - st, inTunnels.length(), (et-st)/inTunnels.length());

    //st = Timer::rdtscSeconds();

    // ����"��Ͳ"��֧-->������ʼ��
    LinkInTunnelToTTunnel( objIds, tObjIds, inTunnels, dg, edges );

    //et = Timer::rdtscSeconds();
    //acutPrintf(_T("\n�����������������֮�����ӷ�Ͳ�����֧,�ķ�ʱ��:%.5f"), et - st);
}

static Digraph::Node AddVirtualSource( Digraph& dg, ObjectIdMap& om )
{
    NodeArray sNodes;  // Դ�㼯��

    // ��������з���,����Դ��
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        // ��¼���=0��Դ��
        if( 0 == countInArcs( dg, n ) ) sNodes.append( n );
    }

    //acutPrintf(_T("\n������Ϸ�֧֮ǰ,��Դ�㡿����:%d"), sNodes.length());

    /*
     * !!!ǰ������!!!
     * ������һ��Դ���һ�����
     * Ŀǰ��ʱ������������"���������"
     * ���磺û��Դ���û�л�㣬����Դ��㶼û��
     */
    if( sNodes.isEmpty() ) return INVALID;

    // ����һ������Դ��
    Digraph::Node sn = dg.addNode();

    // ����Դ��"������֧"(����Դ��-->Դ��)
    int len = sNodes.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = dg.addArc( sn, sNodes[i] );
        om[e] = AcDbObjectId::kNull;
    }
    return sn;  // ��������������Դ��
}

static Digraph::Node AddVirtualTarget( Digraph& dg, ObjectIdMap& om )
{
    NodeArray tNodes;  // Դ�㼯��

    // ��������з���,���һ��
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        // ��¼����=0�Ļ��
        if( 0 == countOutArcs( dg, n ) ) tNodes.append( n );
    }

    //acutPrintf(_T("\n������Ϸ�֧֮ǰ,����㡿����:%d"), tNodes.length());

    /*
     * !!!ǰ������!!!
     * ������һ��Դ���һ�����
     * Ŀǰ��ʱ������������"���������"
     * ���磺û��Դ���û�л�㣬����Դ��㶼û��
     */

    if( tNodes.isEmpty() ) return INVALID;

    // ����һ��������
    Digraph::Node tn = dg.addNode();

    // ���ӻ��"�����֧"(���-->������)
    int len = tNodes.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = dg.addArc( tNodes[i], tn );
        om[e] = AcDbObjectId::kNull;
    }
    return tn; // ����������������
}

bool BuildNetwork( Digraph& dg, ObjectIdMap& om )
{
    AcDbObjectIdArray objIds;
    double st = Timer::rdtscSeconds();

    // 1) �������еķ�֧ͼԪ
    FindEdgeGEs( objIds );

    double et = Timer::rdtscSeconds();
    acutPrintf( _T( "\n���ҷ�֧ͼԪ�ķ�ʱ��:%.5f\n" ), et - st );

    if( objIds.isEmpty() ) return false;

    st = Timer::rdtscSeconds();

    // 2) ��ȡ���еķ�֧ͼԪ��ʼĩ������
    AcGePoint3dArray spts, epts;
    GetEdgeSEPts( objIds, spts, epts );
    if( spts.length() != objIds.length() ) return false;

    // ����ʼĩ�����꣬��ȡ�ڵ�����
    AcGePoint3dArray pts;
    GetNodePts( spts, epts, pts );

    // 3) ����������ͽڵ��ӳ��
    NodeArray nodes;
    BuildNodesMap( pts, dg, nodes );

    // 4) ����ͼԪ�ͷ�֧��ӳ��
    EdgeArray edges;
    BuildEdgesMap( objIds, pts, dg, nodes, edges );

    et = Timer::rdtscSeconds();
    acutPrintf( _T( "\n��������ķ�ʱ��:%.5f\n" ), et - st );

    st = Timer::rdtscSeconds();

    // 5) �ھ��������͵������֮�����������֧
    DealWithTTunnel( objIds, dg, edges );

    et = Timer::rdtscSeconds();
    acutPrintf( _T( "\n������������ķ�ʱ��:%.5f\n" ), et - st );

    // 6) �����֧����idӳ��om
    int len = edges.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = edges[i];
        om[e] = objIds[i];
    }

    acutPrintf( _T( "\nԭʼͼ�ڵ����:%d ��֧����:%d" ), countNodes( dg ), countArcs( dg ) );

    return true;
}

bool AddVirtualSTNode( Digraph& dg, ObjectIdMap& om, Digraph::Node& sn, Digraph::Node& tn )
{
    // ����һ������Դ�㼰��֧
    /*Digraph::Node*/ sn = AddVirtualSource( dg, om );

    // ����һ�������㼰��֧
    /*Digraph::Node*/
    tn = AddVirtualTarget( dg, om );

    return ( sn != INVALID ) && ( tn != INVALID );
}

static void GetHosts( const AcDbObjectIdArray& objIds, AcDbObjectIdArray& hosts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        TagGE* pTag = TagGE::cast( pObj );
        if( pTag == 0 ) continue;

        AcDbObjectId host = pTag->getRelatedGE();
        if( !host.isNull() ) hosts.append( host );
    }
    actrTransactionManager->endTransaction();
}

void FilterBlockEdges( Digraph& dg, ObjectIdMap& om, ArcFilter& bf )
{
    // ȫ����ʼ��Ϊtrue
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        bf[e] = true;
    }

    // �������еķ��ź��ܱչ�����
    AcDbObjectIdArray tagObjIds;
    FindBlocks( tagObjIds );
    if( tagObjIds.isEmpty() ) return;

    // ���ҹ�������������
    AcDbObjectIdArray hosts;
    GetHosts( tagObjIds, hosts );

    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        AcDbObjectId objId = om[e];
        if( objId.isNull() )
        {
            bf[e] = true;
        }
        else
        {
            bf[e] = !hosts.contains( objId );
        }
    }
}

void EraseBlockEdges( Digraph& dg, ArcFilter& bf )
{
    EdgeArray es;
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( !bf[e] ) es.append( e );
    }

    int len = es.length();
    for( int i = 0; i < len; i++ )
    {
        dg.erase( es[i] );
    }

    acutPrintf( _T( "\nɾ����Ϸ�֧���ͼ�ڵ����:%d ��֧����:%d" ), countNodes( dg ), countArcs( dg ) );
}

bool GetNodeGeoData( Digraph& dg, ObjectIdMap& om, NodeGeoDataMap& ngm )
{
    // ��ʼ��Ϊԭ��(0,0,0)
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        ngm[n] = AcGePoint3d::kOrigin;
    }

    // �������е�����Լ���Ӧ�ķ�֧
    AcDbObjectIdArray objIds;
    EdgeArray es;
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        AcDbObjectId objId = om[e];
        if( !objId.isNull() )
        {
            es.append( e );
            objIds.append( objId );
        }
    }

    // �������������ʼĩ������
    AcGePoint3dArray spts, epts;
    GetEdgeSEPts( objIds, spts, epts );
    if( spts.length() != objIds.length() ) return false;

    int len = es.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = es[i];

        ngm[dg.source( e )] = spts[i]; // ʼ�ڵ�����
        ngm[dg.target( e )] = epts[i]; // ĩ�ڵ�����
    }

    return true;
}