#include "StdAfx.h"
#include "ARX_VNC.h"

#include "NetworkDataLink.h"

#include "../GraphTool/BuildNetwork.h"
#include "../GraphTool/Tool.h"
#include "../VNC/VNC.h"

typedef Digraph::ArcMap<EdgeDataLink*> EdgeDataLinkMap;
typedef Digraph::ArcMap<FanDataLink*> FanDataLinkMap;

// ������д��û��̫�������
#define DATA_COPY(pD, pDL, x) pD->x = pDL->x

// ���Ҿ��������֮ǰ����������֧
static bool FindVirtualEdgesBeforeTTunnels( Digraph& dg, ObjectIdMap& om, const AcDbObjectIdArray& tObjIds, EdgeArray& es )
{
    AcDbObjectIdArray tObjIds2; // ��¼���������
    EdgeArray es2;            // ��¼���������֮ǰ���ӵ������֧
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        AcDbObjectId objId = om[e];
        if( objId.isNull() ) continue;
        if( !tObjIds.contains( objId ) ) continue;

        tObjIds2.append( objId );
        for( Digraph::InArcIt ie( dg, dg.source( e ) ); ie != INVALID; ++ie )
        {
            // idΪ�յķ�֧��Ϊ���ӵ������֧
            if( om[ie].isNull() )
            {
                es2.append( ie );
                break;
            }
        }
    }

    if( tObjIds2.length() != tObjIds.length() ) return false;
    if( tObjIds2.length() != es2.length() ) return false;

    // ����˳��
    int len = tObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        int pos = tObjIds2.find( tObjIds[i] );
        es.append( es2[pos] );
    }

    return true;
}

static EdgeDataLink* CreatEdgeDataLink( const AcDbObjectId& objId )
{
    EdgeDataLink* pEdgeData = new EdgeDataLink();
    pEdgeData->setDataSource( objId );
    if( !pEdgeData->updateData( false ) )
    {
        acutPrintf( _T( "\n��ȡ�������ʧ��..." ) );
        delete pEdgeData;
        pEdgeData = 0;
    }
    return pEdgeData;
}

static FanDataLink* CreateFanDataLink( const AcDbObjectId& objId )
{
    FanDataLink* pFanData = new FanDataLink();
    pFanData->setDataSource( objId );
    if( !pFanData->updateData( false ) )
    {
        acutPrintf( _T( "\n��ȡ�������ʧ��..." ) );
        delete pFanData;
        pFanData = 0;
    }
    return pFanData;
}

static GateDataLink* CreateGateDataLink( const AcDbObjectId& objId )
{
    GateDataLink* pGateData = new GateDataLink();
    pGateData->setDataSource( objId );
    if( !pGateData->updateData( false ) )
    {
        acutPrintf( _T( "\n��ȡ����������ʧ��..." ) );
        delete pGateData;
        pGateData = 0;
    }
    return pGateData;
}

static ChimneyDataLink* CreateChimneyDataLink( const AcDbObjectId& objId )
{
    ChimneyDataLink* pChimneyData = new ChimneyDataLink();
    pChimneyData->setDataSource( objId );
    if( !pChimneyData->updateData( false ) )
    {
        acutPrintf( _T( "\n��ȡ��Ͳ����ʧ��..." ) );
        delete pChimneyData;
        pChimneyData = 0;
    }
    return pChimneyData;
}

static bool InitEdgeDatas( Digraph& dg, ObjectIdMap& om, EdgeDataLinkMap& edl )
{
    bool ret = true;
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        EdgeDataLink* pEdgeData = 0;

        if( om[e].isNull() )
        {
            // ��֧������Ϊ0
            pEdgeData = new EdgeDataLink();
        }
        else
        {
            // ��ͼԪ����ȡ����
            pEdgeData = CreatEdgeDataLink( om[e] );
        }

        if( pEdgeData == 0 )
        {
            ret = false;
            break;
        }
        edl[e] = pEdgeData;
    }
    return ret;
}

static void InitAllFanDatas( Digraph& dg, FanDataLinkMap& fdl )
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        fdl[e] = 0; // ��ʼ��Ϊ0
    }
}

static bool InitMainFanDatas( Digraph& dg, ObjectIdMap& om, FanDataLinkMap& fdl )
{
    // �������е���Ҫͨ���
    AcDbObjectIdArray fanObjIds;
    FindMainFans( fanObjIds );
    if( fanObjIds.isEmpty() ) return true;

    // ������Ҫͨ��������������
    AcDbObjectIdArray hosts;
    GetHosts( fanObjIds, hosts );

    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        int pos = hosts.find( om[e] );
        if( pos == -1 ) continue;

        if( fdl[e] != 0 ) continue; // ��֧�ѹ������(Ŀǰ��ʱ������һ����֧�����������!!!)

        FanDataLink* pFanData = CreateFanDataLink( fanObjIds[pos] );
        if( pFanData == 0 ) return false; // ���·������ʧ��!!!

        fdl[e] = pFanData;               // ��֧����һ���������
    }
    return true;
}

// ��ʼ���ֲ�ͨ����������ľ���������뵼�����֮��������֧�Ĳ���
// ��Ҫ��ȡ�ֲ�ͨ����ͷ�Ͳ������
static bool InitLocalFanDatas( Digraph& dg, ObjectIdMap& om,
                               EdgeDataLinkMap& edl, FanDataLinkMap& fdl )
{
    // �������еľ��������
    AcDbObjectIdArray tObjIds;
    FindTTunnels( tObjIds );
    if( tObjIds.isEmpty() ) return true;

    // �������еľֲ�ͨ���
    // ���FindLocalFans����true
    // ������(tObjIds.length() == fanObjIds.length()) ==> true
    AcDbObjectIdArray fanObjIds;
    FindLocalFans( tObjIds, fanObjIds );

    // �������еķ�Ͳ
    // �������FindChimneystrue��
    // ������(tObjIds.length() == cObjIds.length()) ==> true
    AcDbObjectIdArray cObjIds;
    FindChimneys( tObjIds, cObjIds );

    // �����ھ��������֮ǰ��ӵ������֧
    EdgeArray es;
    if( !FindVirtualEdgesBeforeTTunnels( dg, om, tObjIds, es ) )
    {
        acutPrintf( _T( "\n���Ҿ���������뵼�����֮��������֧ʧ��" ) );
        return false;
    }

    int len = es.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = es[i];

        FanDataLink* pFanData = CreateFanDataLink( fanObjIds[i] );
        if( pFanData == 0 ) return false; // ��ȡ�������ʧ��!!!

        fdl[e] = pFanData;              // ���ֲ�ͨ������ݹ�������֧��

        ChimneyDataLink* pChimneyData = CreateChimneyDataLink( cObjIds[i] );
        if( pChimneyData == 0 ) return false; // ��ȡ��Ͳ����ʧ��

        EdgeDataLink* pEdgeData = edl[e];
        pEdgeData->r = pChimneyData->r;   // ����Ͳ�ķ��踳�������֧

        delete pChimneyData;              // �����ڴ�(��Ͳ����ֻ����ʱ��)
    }
    return true;
}

static bool InitGateEdges( Digraph& dg, ObjectIdMap& om, EdgeDataLinkMap& edl )
{
    // �������еĹ�����(���źͷ細)
    AcDbObjectIdArray gateObjIds;
    FindGates( gateObjIds );
    if( gateObjIds.isEmpty() ) return true;

    // ���ҹ����������������
    AcDbObjectIdArray hosts;
    GetHosts( gateObjIds, hosts );

    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        int pos = hosts.find( om[e] );
        if( pos == -1 ) continue;

        GateDataLink* pGateData = CreateGateDataLink( gateObjIds[pos] );
        if( pGateData == 0 ) return false; // ���¹���������ʧ��!!!

        edl[e]->r += pGateData->r;        // ��������ķ����ۼӵ���֧������

        delete pGateData;
    }
    return true;
}

static bool InitNetwok( Digraph& dg, EdgeDataLinkMap& edl, FanDataLinkMap& fdl )
{
    // ��������
    ObjectIdMap om( dg );
    if( !BuildNetwork( dg, om ) )
    {
        acutPrintf( _T( "\n��������ʧ��" ) );
        return false;
    }

    if( !InitEdgeDatas( dg, om, edl ) ) return false;	    // ��ʼ����֧����
    InitAllFanDatas( dg, fdl );                             // ��ʼ�����з�֧�Ĺ����������Ϊ0
    if( !InitMainFanDatas( dg, om, fdl ) ) return false;    // ��ʼ����Ҫͨ����������������
    if( !InitLocalFanDatas( dg, om, edl, fdl ) ) return false; // ��ʼ����������������ӵ������֧����
    if( !InitGateEdges( dg, om, edl ) ) return false;       // ��ʼ����Ϸ�֧����(�޸�ԭʼ��֧�ķ���)

    return true;
}

static void CreateEdgeDatas( Digraph& dg, EdgeDataLinkMap& edl, VNC_EdgeDataMap& ed )
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        VNC_EdgeData* pD = new VNC_EdgeData();
        EdgeDataLink* pDL = edl[e];

        DATA_COPY( pD, pDL, r );
        DATA_COPY( pD, pDL, ar );
        DATA_COPY( pD, pDL, q );
        DATA_COPY( pD, pDL, fq );

        ed[e] = pD;
    }
}

static void CreateFanDatas( Digraph& dg, FanDataLinkMap& fdl, VNC_FanDataMap& fd )
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( fdl[e] == 0 )
        {
            fd[e] = 0;
        }
        else
        {
            VNC_FanData* pD = new VNC_FanData();
            FanDataLink* pDL = fdl[e];

            DATA_COPY( pD, pDL, a0 );
            DATA_COPY( pD, pDL, a1 );
            DATA_COPY( pD, pDL, a2 );
            DATA_COPY( pD, pDL, a3 );
            DATA_COPY( pD, pDL, a4 );
            DATA_COPY( pD, pDL, a5 );

            fd[e] = pD;
        }
    }
}

static void ReadEdgeDatas( Digraph& dg, EdgeDataLinkMap& edl, VNC_EdgeDataMap& ed )
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        VNC_EdgeData* pD = ed[e];
        EdgeDataLink* pDL = edl[e];

        DATA_COPY( pDL, pD, r );
        DATA_COPY( pDL, pD, ar );
        DATA_COPY( pDL, pD, q );
        DATA_COPY( pDL, pD, fq );
    }
}

static void ReadFanDatas( Digraph& dg, FanDataLinkMap& fdl, VNC_FanDataMap& fd )
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( fd[e] == 0 ) continue;

        VNC_FanData* pD = fd[e];
        FanDataLink* pDL = fdl[e];

        DATA_COPY( pDL, pD, a0 );
        DATA_COPY( pDL, pD, a1 );
        DATA_COPY( pDL, pD, a2 );
        DATA_COPY( pDL, pD, a3 );
        DATA_COPY( pDL, pD, a4 );
        DATA_COPY( pDL, pD, a5 );
    }
}

static void ClearAllVNCDatas( Digraph& dg, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd )
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        delete ed[e];
        ed[e] = 0;
        delete fd[e];
        fd[e] = 0;
    }
}

static void ClearAllDataLinks( Digraph& dg, EdgeDataLinkMap& edl, FanDataLinkMap& fdl )
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        delete edl[e];
        edl[e] = 0;
        delete fdl[e];
        fdl[e] = 0;
    }
}

static void UpdateAllDataLinks( Digraph& dg, EdgeDataLinkMap& edl, FanDataLinkMap& fdl )
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( edl[e] != 0 )
        {
            edl[e]->updateData( true );
        }

        if( fdl[e] != 0 )
        {
            fdl[e]->updateData( true );
        }
    }
}

void ARX_VNC()
{
    Digraph dg;
    EdgeDataLinkMap edl( dg );
    FanDataLinkMap fdl( dg );
    if( !InitNetwok( dg, edl, fdl ) )
    {
        acutPrintf( _T( "\n��ʼ������ʧ��!" ) );
        return;
    }

    VNC_EdgeDataMap ed( dg );
    CreateEdgeDatas( dg, edl, ed );
    VNC_FanDataMap fd( dg );
    CreateFanDatas( dg, fdl, fd );

    // ����VNC.dllģ��
    bool ret = VNC( dg, ed, fd );;
    if( ret )
    {
        ReadEdgeDatas( dg, edl, ed );
        ReadFanDatas( dg, fdl, fd );
        UpdateAllDataLinks( dg, edl, fdl );
        acutPrintf( _T( "\n����vncģ��ɹ�!" ) );
    }

    ClearAllVNCDatas( dg, ed, fd );
    ClearAllDataLinks( dg, edl, fdl );
}