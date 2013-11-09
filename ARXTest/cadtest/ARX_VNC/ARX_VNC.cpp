#include "StdAfx.h"
#include "ARX_VNC.h"

#include "NetworkDataLink.h"

#include "../GraphTool/BuildNetwork.h"
#include "../GraphTool/Tool.h"
#include "../VNC/VNC.h"

typedef Digraph::ArcMap<EdgeDataLink*> EdgeDataLinkMap;
typedef Digraph::ArcMap<FanDataLink*> FanDataLinkMap;

// 方便书写，没有太大的作用
#define DATA_COPY(pD, pDL, x) pD->x = pDL->x

// 查找掘进工作面之前的入边虚拟分支
static bool FindVirtualEdgesBeforeTTunnels( Digraph& dg, ObjectIdMap& om, const AcDbObjectIdArray& tObjIds, EdgeArray& es )
{
    AcDbObjectIdArray tObjIds2; // 记录掘进工作面
    EdgeArray es2;            // 记录掘进工作面之前增加的虚拟分支
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        AcDbObjectId objId = om[e];
        if( objId.isNull() ) continue;
        if( !tObjIds.contains( objId ) ) continue;

        tObjIds2.append( objId );
        for( Digraph::InArcIt ie( dg, dg.source( e ) ); ie != INVALID; ++ie )
        {
            // id为空的分支即为增加的虚拟分支
            if( om[ie].isNull() )
            {
                es2.append( ie );
                break;
            }
        }
    }

    if( tObjIds2.length() != tObjIds.length() ) return false;
    if( tObjIds2.length() != es2.length() ) return false;

    // 调整顺序
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
        acutPrintf( _T( "\n提取巷道数据失败..." ) );
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
        acutPrintf( _T( "\n提取风机数据失败..." ) );
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
        acutPrintf( _T( "\n提取构筑物数据失败..." ) );
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
        acutPrintf( _T( "\n提取风筒数据失败..." ) );
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
            // 分支参数均为0
            pEdgeData = new EdgeDataLink();
        }
        else
        {
            // 从图元中提取数据
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
        fdl[e] = 0; // 初始化为0
    }
}

static bool InitMainFanDatas( Digraph& dg, ObjectIdMap& om, FanDataLinkMap& fdl )
{
    // 查找所有的主要通风机
    AcDbObjectIdArray fanObjIds;
    FindMainFans( fanObjIds );
    if( fanObjIds.isEmpty() ) return true;

    // 查找主要通风机所关联的巷道
    AcDbObjectIdArray hosts;
    GetHosts( fanObjIds, hosts );

    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        int pos = hosts.find( om[e] );
        if( pos == -1 ) continue;

        if( fdl[e] != 0 ) continue; // 分支已关联风机(目前暂时不考虑一条分支多个风机的情况!!!)

        FanDataLink* pFanData = CreateFanDataLink( fanObjIds[pos] );
        if( pFanData == 0 ) return false; // 更新风机数据失败!!!

        fdl[e] = pFanData;               // 分支关联一个风机数据
    }
    return true;
}

// 初始化局部通风机所关联的掘进工作面与导风巷道之间的虚拟分支的参数
// 需要读取局部通风机和风筒的数据
static bool InitLocalFanDatas( Digraph& dg, ObjectIdMap& om,
                               EdgeDataLinkMap& edl, FanDataLinkMap& fdl )
{
    // 查找所有的掘进工作面
    AcDbObjectIdArray tObjIds;
    FindTTunnels( tObjIds );
    if( tObjIds.isEmpty() ) return true;

    // 查找所有的局部通风机
    // 如果FindLocalFans返回true
    // 则隐含(tObjIds.length() == fanObjIds.length()) ==> true
    AcDbObjectIdArray fanObjIds;
    FindLocalFans( tObjIds, fanObjIds );

    // 查找所有的风筒
    // 如果返回FindChimneystrue，
    // 则隐含(tObjIds.length() == cObjIds.length()) ==> true
    AcDbObjectIdArray cObjIds;
    FindChimneys( tObjIds, cObjIds );

    // 查找在掘进工作面之前添加的虚拟分支
    EdgeArray es;
    if( !FindVirtualEdgesBeforeTTunnels( dg, om, tObjIds, es ) )
    {
        acutPrintf( _T( "\n查找掘进工作面与导风巷道之间的虚拟分支失败" ) );
        return false;
    }

    int len = es.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = es[i];

        FanDataLink* pFanData = CreateFanDataLink( fanObjIds[i] );
        if( pFanData == 0 ) return false; // 提取风机数据失败!!!

        fdl[e] = pFanData;              // 将局部通风机数据关联到分支上

        ChimneyDataLink* pChimneyData = CreateChimneyDataLink( cObjIds[i] );
        if( pChimneyData == 0 ) return false; // 提取风筒数据失败

        EdgeDataLink* pEdgeData = edl[e];
        pEdgeData->r = pChimneyData->r;   // 将风筒的风阻赋给虚拟分支

        delete pChimneyData;              // 销毁内存(风筒数据只作临时用)
    }
    return true;
}

static bool InitGateEdges( Digraph& dg, ObjectIdMap& om, EdgeDataLinkMap& edl )
{
    // 查找所有的构筑物(风门和风窗)
    AcDbObjectIdArray gateObjIds;
    FindGates( gateObjIds );
    if( gateObjIds.isEmpty() ) return true;

    // 查找构筑物所关联的巷道
    AcDbObjectIdArray hosts;
    GetHosts( gateObjIds, hosts );

    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        int pos = hosts.find( om[e] );
        if( pos == -1 ) continue;

        GateDataLink* pGateData = CreateGateDataLink( gateObjIds[pos] );
        if( pGateData == 0 ) return false; // 更新构筑物数据失败!!!

        edl[e]->r += pGateData->r;        // 将构筑物的风阻累加到分支风阻上

        delete pGateData;
    }
    return true;
}

static bool InitNetwok( Digraph& dg, EdgeDataLinkMap& edl, FanDataLinkMap& fdl )
{
    // 创建网络
    ObjectIdMap om( dg );
    if( !BuildNetwork( dg, om ) )
    {
        acutPrintf( _T( "\n创建网络失败" ) );
        return false;
    }

    if( !InitEdgeDatas( dg, om, edl ) ) return false;	    // 初始化分支数据
    InitAllFanDatas( dg, fdl );                             // 初始化所有分支的关联风机数据为0
    if( !InitMainFanDatas( dg, om, fdl ) ) return false;    // 初始化主要通风机所在巷道的数据
    if( !InitLocalFanDatas( dg, om, edl, fdl ) ) return false; // 初始化处理掘进工作面添加的虚拟分支数据
    if( !InitGateEdges( dg, om, edl ) ) return false;       // 初始化阻断分支数据(修改原始分支的风阻)

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
        acutPrintf( _T( "\n初始化网络失败!" ) );
        return;
    }

    VNC_EdgeDataMap ed( dg );
    CreateEdgeDatas( dg, edl, ed );
    VNC_FanDataMap fd( dg );
    CreateFanDatas( dg, fdl, fd );

    // 调用VNC.dll模块
    bool ret = VNC( dg, ed, fd );;
    if( ret )
    {
        ReadEdgeDatas( dg, edl, ed );
        ReadFanDatas( dg, fdl, fd );
        UpdateAllDataLinks( dg, edl, fdl );
        acutPrintf( _T( "\n调用vnc模块成功!" ) );
    }

    ClearAllVNCDatas( dg, ed, fd );
    ClearAllDataLinks( dg, edl, fdl );
}