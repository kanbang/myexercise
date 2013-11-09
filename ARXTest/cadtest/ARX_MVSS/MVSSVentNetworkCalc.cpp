#include "StdAfx.h"

#include "MVSSVentNetworkCalc.h"
#include "MVSSData.h"

#include "../MineGE/DataHelper.h"

#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"
#include "../DaoHelper/DataReader.h"

#include "../GraphTool/Tool.h"

// 调用mvss网络解算模块
#include "ADCalNetwork.h"
#pragma comment(lib,"DLJCalNetWork.lib")

// 方便书写，没有太大的作用
#define DATA_COPY(pD, pDL, x) pD->x = pDL->x

typedef AcArray<MVSSData*> MVSSDataArray;

static MVSSDataLink* CreateDataLink( MVSS_GETYPE mg, const AcDbObjectId& objId )
{
    MVSSDataLink* pDL = 0;
    switch( mg )
    {
    case MG_TUNNEL:
        pDL = new TunnelDataLink();
        break;

    case MG_GATE:
        pDL = new GateDataLink();
        break;

    case MG_FAN:
        pDL = new FanDataLink();
        break;

        //case MG_JOINT:
        //	pDL = new JointDataLink();
        //	break;
    }

    // id等于null的图元也是有效的
    if( !objId.isNull() )
    {
        pDL->setDataSource( objId );
        if( !pDL->updateData( false ) )
        {
            delete pDL;
            pDL = 0;
        }
    }
    return pDL;
}

static bool CreateTunnelDataLinks( Digraph& dg, ObjectIdMap& om, MVSSDataLinkArray& m_TDs )
{
    bool ret = true;
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        AcDbObjectId objId = om[e];
        if( objId.isNull() ) continue;

        MVSSDataLink* pD = CreateDataLink( MG_TUNNEL, objId );
        if( pD == 0 )
        {
            ret = false;
            break;
        }

        TunnelDataLink* pTD = ( TunnelDataLink* )pD;
        pTD->objId = Digraph::id( e );
        pTD->spt = Digraph::id( dg.source( e ) );
        pTD->ept = Digraph::id( dg.target( e ) );

        m_TDs.append( pTD );
    }
    return ret;
}

static bool CreateJointDataLinks( Digraph& dg, NodeGeoDataMap& ngm, MVSSDataLinkArray& m_JDs )
{
    bool ret = true;
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        JointDataLink* pJD = new JointDataLink();
        //pJD->setDataSource(); // 不需要关联
        pJD->objId = Digraph::id( n );
        pJD->x = ngm[n].x;
        pJD->y = ngm[n].y;
        pJD->z = ngm[n].z;

        m_JDs.append( pJD );
    }
    return ret;
}

static Digraph::Arc GetEdgeById( Digraph& dg, ObjectIdMap& om, const AcDbObjectId& objId )
{
    Digraph::Arc ee = INVALID;
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( om[e] == objId )
        {
            ee = e;
            break;
        }
    }
    return ee;
}

static bool CreateGateDataLinks( Digraph& dg, ObjectIdMap& om, MVSSDataLinkArray& m_GDs )
{
    // 查找所有的风门构筑物
    AcDbObjectIdArray objIds;
    FindGates( objIds );
    if( objIds.isEmpty() ) return true;

    // 查找风门关联的巷道
    AcDbObjectIdArray hosts;
    GetHosts( objIds, hosts );
    if( hosts.length() != objIds.length() ) return false;

    // 查找风门的插入点坐标
    AcGePoint3dArray pts;
    GetInsertPts( objIds, pts );
    if( pts.length() != objIds.length() ) return false;

    bool ret = true;
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        MVSSDataLink* pD = CreateDataLink( MG_GATE, objIds[i] );
        if( pD == 0 )
        {
            ret = false;
            break;
        }

        GateDataLink* pGD = ( GateDataLink* )pD;
        pGD->objId = i;
        pGD->tObjId = Digraph::id( GetEdgeById( dg, om, hosts[i] ) );
        pGD->x = pts[i].x;
        pGD->y = pts[i].y;
        pGD->z = pts[i].z;

        m_GDs.append( pGD );
    }
    return ret;
}

static bool CreateMainFanDataLinks( Digraph& dg, ObjectIdMap& om, MVSSDataLinkArray& m_FDs )
{
    // 查找所有的主要通风机
    AcDbObjectIdArray objIds;
    FindMainFans( objIds );
    if( objIds.isEmpty() ) return true;

    // 查找主要通风机关联的巷道
    AcDbObjectIdArray hosts;
    GetHosts( objIds, hosts );
    if( hosts.length() != objIds.length() ) return false;

    // 查找主要通风机的插入点坐标
    AcGePoint3dArray pts;
    GetInsertPts( objIds, pts );
    if( pts.length() != objIds.length() ) return false;

    bool ret = true;
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        MVSSDataLink* pD = CreateDataLink( MG_FAN, objIds[i] );
        if( pD == 0 )
        {
            ret = false;
            break;
        }

        FanDataLink* pFD = ( FanDataLink* )pD;
        pFD->objId = i;
        pFD->tObjId = Digraph::id( GetEdgeById( dg, om, hosts[i] ) );
        pFD->x = pts[i].x;
        pFD->y = pts[i].y;
        pFD->z = pts[i].z;

        m_FDs.append( pFD );
    }
    return ret;
}

// 查找掘进工作面之前的入边虚拟分支
static bool FindVirtualEdgesBeforeTTunnels( Digraph& dg, ObjectIdMap& om, const AcDbObjectIdArray& tObjIds, EdgeArray& es )
{
    AcDbObjectIdArray tObjIds2; // 记录掘进工作面
    EdgeArray es2;              // 记录掘进工作面之前增加的虚拟分支
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        AcDbObjectId objId = om[e];
        if( objId.isNull() || !tObjIds.contains( objId ) ) continue;

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

static void CreateLocalFanEdgeDataLinks( Digraph& dg, const EdgeArray& es, const AcDbObjectIdArray& objIds, MVSSDataLinkArray& m_TDs )
{
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = es[i];
        AcDbObjectId objId = objIds[i];

        // 1) 添加虚拟分支巷道数据
        TunnelDataLink* pTD = new TunnelDataLink();
        pTD->objId = Digraph::id( e );
        pTD->spt = Digraph::id( dg.source( e ) );
        pTD->ept = Digraph::id( dg.target( e ) );

        // 将风筒的"风阻"赋值虚拟分支巷道的"摩擦风阻"
        CString v;
        DataHelper::GetPropertyData( objIds[i], _T( "风阻" ), v );

        if( v.GetLength() == 0 )
        {
            pTD->r = _T( "0" );
        }
        else
        {
            pTD->r = v;
        }

        m_TDs.append( pTD );
    }
}

static AcGePoint3d MidPt( const AcGePoint3d& spt, const AcGePoint3d& ept )
{
    AcGeVector3d v = ept - spt;
    return spt + v * 0.5;
}

static bool CreateLocalFanDataLinks( Digraph& dg, NodeGeoDataMap& ngm,
                                     const EdgeArray& es, const AcDbObjectIdArray& objIds,
                                     MVSSDataLinkArray& m_FDs )
{
    bool ret = true;

    int pos = m_FDs.length();
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = es[i];
        AcDbObjectId objId = objIds[i];

        MVSSDataLink* pD = CreateDataLink( MG_FAN, objId );
        if( pD == 0 )
        {
            ret = false;
            break;
        }

        FanDataLink* pFD = ( FanDataLink* )pD;
        pFD->objId = pos + i;
        pFD->tObjId = Digraph::id( e );

        // 虚分支中点坐标
        AcGePoint3d pt = MidPt( ngm[dg.source( e )], ngm[dg.target( e )] );
        pFD->x = pt.x;
        pFD->y = pt.y;
        pFD->z = pt.z;

        m_FDs.append( pFD );
    }

    return ret;
}

// 查找除了风筒虚拟分支之外的其它虚拟分支
static void FindNullEdges( Digraph& dg, ObjectIdMap& om, const EdgeArray& es, EdgeArray& es2 )
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        AcDbObjectId objId = om[e];
        if( objId.isNull() && !es.contains( e ) ) es2.append( e );
    }
}

static void CreateNullTunnelDataLinks( Digraph& dg, const EdgeArray& es, MVSSDataLinkArray& m_TDs )
{
    int len = es.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = es[i];

        TunnelDataLink* pTD = new TunnelDataLink();
        pTD->objId = Digraph::id( e );
        pTD->spt = Digraph::id( dg.source( e ) );
        pTD->ept = Digraph::id( dg.target( e ) );

        m_TDs.append( pTD );
    }
}

static bool CreateLocalFanDataLinks( Digraph& dg, ObjectIdMap& om, NodeGeoDataMap& ngm, MVSSDataLinkArray& m_TDs, MVSSDataLinkArray& m_FDs )
{
    // 查找所有的掘进工作面
    AcDbObjectIdArray tObjIds;
    FindTTunnels( tObjIds );
    if( tObjIds.isEmpty() ) return true;

    // 查找所有的局部通风机
    AcDbObjectIdArray fanObjIds;
    FindLocalFans( tObjIds, fanObjIds );

    // 查找所有的风筒
    AcDbObjectIdArray cObjIds;
    FindChimneys( tObjIds, cObjIds );

    // 查找在掘进工作面之前添加的虚拟分支
    EdgeArray es;
    if( !FindVirtualEdgesBeforeTTunnels( dg, om, tObjIds, es ) )
    {
        acutPrintf( _T( "\n查找掘进工作面导风虚分支失败" ) );
        return false;
    }

    // 将风筒的数据(风阻)附加到虚拟分支上
    CreateLocalFanEdgeDataLinks( dg, es, cObjIds, m_TDs );

    // 将局部扇风机附加到虚拟分支上
    if( !CreateLocalFanDataLinks( dg, ngm, es, fanObjIds, m_FDs ) )
    {
        acutPrintf( _T( "\n局部扇风机附加到虚拟分支上失败" ) );
        return false;
    }

    EdgeArray es2;
    FindNullEdges( dg, om, es, es2 );
    CreateNullTunnelDataLinks( dg, es2, m_TDs );

    return true;
}

static void CreateTunnelData( const MVSSDataLinkArray& datas, MVSSDataArray& td )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        TunnelDataLink* pDL = ( TunnelDataLink* )datas[i];
        TunnelData* pTD = new TunnelData();

        DATA_COPY( pTD, pDL, objId );
        DATA_COPY( pTD, pDL, spt );
        DATA_COPY( pTD, pDL, ept );

        DATA_COPY( pTD, pDL, name );
        DATA_COPY( pTD, pDL, q );
        DATA_COPY( pTD, pDL, r );
        DATA_COPY( pTD, pDL, lr );
        DATA_COPY( pTD, pDL, h );
        DATA_COPY( pTD, pDL, fq );
        DATA_COPY( pTD, pDL, rho );
        DATA_COPY( pTD, pDL, ad );
        DATA_COPY( pTD, pDL, ah );
        DATA_COPY( pTD, pDL, ar );
        DATA_COPY( pTD, pDL, rho1 );
        DATA_COPY( pTD, pDL, rho2 );
        DATA_COPY( pTD, pDL, rho3 );
        DATA_COPY( pTD, pDL, rho4 );
        DATA_COPY( pTD, pDL, tq );
        DATA_COPY( pTD, pDL, th );
        DATA_COPY( pTD, pDL, l );
        DATA_COPY( pTD, pDL, apha );
        DATA_COPY( pTD, pDL, r100 );
        DATA_COPY( pTD, pDL, atype );
        DATA_COPY( pTD, pDL, area );
        DATA_COPY( pTD, pDL, c );
        DATA_COPY( pTD, pDL, st );
        DATA_COPY( pTD, pDL, maxV );
        DATA_COPY( pTD, pDL, tUse );
        DATA_COPY( pTD, pDL, tRegion );
        DATA_COPY( pTD, pDL, temp );
        DATA_COPY( pTD, pDL, humidity );
        DATA_COPY( pTD, pDL, O2 );
        DATA_COPY( pTD, pDL, CH4 );
        DATA_COPY( pTD, pDL, CO2 );
        DATA_COPY( pTD, pDL, CO );
        DATA_COPY( pTD, pDL, reliability );
        DATA_COPY( pTD, pDL, type );
        DATA_COPY( pTD, pDL, comment );
        DATA_COPY( pTD, pDL, eh );

        td.append( pTD );
    }
}

static void CreateJointData( const MVSSDataLinkArray& datas, MVSSDataArray& jd )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        JointDataLink* pDL = ( JointDataLink* )datas[i];
        JointData* pJD = new JointData();

        DATA_COPY( pJD, pDL, objId );

        DATA_COPY( pJD, pDL, x );
        DATA_COPY( pJD, pDL, y );
        DATA_COPY( pJD, pDL, z );

        DATA_COPY( pJD, pDL, name );
        DATA_COPY( pJD, pDL, rp );
        DATA_COPY( pJD, pDL, p );
        DATA_COPY( pJD, pDL, comment );

        jd.append( pJD );
    }
}

static void CreateGateData( const MVSSDataLinkArray& datas, MVSSDataArray& gd )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        GateDataLink* pDL = ( GateDataLink* )datas[i];
        GateData* pGD = new GateData();

        DATA_COPY( pGD, pDL, objId );
        DATA_COPY( pGD, pDL, tObjId );

        DATA_COPY( pGD, pDL, x );
        DATA_COPY( pGD, pDL, y );
        DATA_COPY( pGD, pDL, z );

        DATA_COPY( pGD, pDL, name );
        DATA_COPY( pGD, pDL, rq );
        DATA_COPY( pGD, pDL, flow );
        DATA_COPY( pGD, pDL, er );
        DATA_COPY( pGD, pDL, ea );
        DATA_COPY( pGD, pDL, trq );

        gd.append( pGD );
    }
}

static void CreateFanData( const MVSSDataLinkArray& datas, MVSSDataArray& fd )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        FanDataLink* pDL = ( FanDataLink* )datas[i];
        FanData* pFD = new FanData();

        DATA_COPY( pFD, pDL, objId );
        DATA_COPY( pFD, pDL, tObjId );

        DATA_COPY( pFD, pDL, x );
        DATA_COPY( pFD, pDL, y );
        DATA_COPY( pFD, pDL, z );

        DATA_COPY( pFD, pDL, name );
        DATA_COPY( pFD, pDL, rs );
        DATA_COPY( pFD, pDL, q );
        DATA_COPY( pFD, pDL, p );
        DATA_COPY( pFD, pDL, power );
        DATA_COPY( pFD, pDL, efficiency );
        DATA_COPY( pFD, pDL, r );
        DATA_COPY( pFD, pDL, aHole );
        DATA_COPY( pFD, pDL, model );
        DATA_COPY( pFD, pDL, angle );
        DATA_COPY( pFD, pDL, rotateSpeed );
        DATA_COPY( pFD, pDL, fq );
        DATA_COPY( pFD, pDL, nq );
        DATA_COPY( pFD, pDL, lq );
        DATA_COPY( pFD, pDL, fsp );
        DATA_COPY( pFD, pDL, ffp );
        DATA_COPY( pFD, pDL, outVP );
        DATA_COPY( pFD, pDL, inVP );
        DATA_COPY( pFD, pDL, inSP );
        DATA_COPY( pFD, pDL, tSP );
        DATA_COPY( pFD, pDL, cPower );
        DATA_COPY( pFD, pDL, inPower );
        DATA_COPY( pFD, pDL, outPower );
        DATA_COPY( pFD, pDL, nPower );
        DATA_COPY( pFD, pDL, lPower );
        DATA_COPY( pFD, pDL, uPower );
        DATA_COPY( pFD, pDL, ePower );
        DATA_COPY( pFD, pDL, sPower );
        DATA_COPY( pFD, pDL, fPower );
        DATA_COPY( pFD, pDL, fr );
        DATA_COPY( pFD, pDL, nr );
        DATA_COPY( pFD, pDL, lr );
        DATA_COPY( pFD, pDL, fma );
        DATA_COPY( pFD, pDL, fmv );
        DATA_COPY( pFD, pDL, outArea );
        DATA_COPY( pFD, pDL, outV );
        DATA_COPY( pFD, pDL, iq );
        DATA_COPY( pFD, pDL, comment );
        DATA_COPY( pFD, pDL, H_a0 );
        DATA_COPY( pFD, pDL, H_a1 );
        DATA_COPY( pFD, pDL, H_a2 );
        DATA_COPY( pFD, pDL, H_a3 );
        DATA_COPY( pFD, pDL, H_a4 );
        DATA_COPY( pFD, pDL, H_a5 );
        DATA_COPY( pFD, pDL, N_a0 );
        DATA_COPY( pFD, pDL, N_a1 );
        DATA_COPY( pFD, pDL, N_a2 );
        DATA_COPY( pFD, pDL, N_a3 );
        DATA_COPY( pFD, pDL, N_a4 );
        DATA_COPY( pFD, pDL, N_a5 );
        DATA_COPY( pFD, pDL, E_a0 );
        DATA_COPY( pFD, pDL, E_a1 );
        DATA_COPY( pFD, pDL, E_a2 );
        DATA_COPY( pFD, pDL, E_a3 );
        DATA_COPY( pFD, pDL, E_a4 );
        DATA_COPY( pFD, pDL, E_a5 );

        fd.append( pFD );
    }
}

// 从MVSSData中读取数据，并更新到节点中
static void ReadJointFromMVSSData( const MVSSDataArray& jd, MVSSDataLinkArray& datas )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        JointDataLink* pDL = ( JointDataLink* )datas[i];
        JointData* pJD = ( JointData* )jd[i];
        //DATA_COPY(pDL, pJD, x);
        //DATA_COPY(pDL, pJD, y);
        //DATA_COPY(pDL, pJD, z);
        DATA_COPY( pDL, pJD, name );
        DATA_COPY( pDL, pJD, rp );
        DATA_COPY( pDL, pJD, p );
        DATA_COPY( pDL, pJD, comment );
    }
}

// 从MVSSData中读取数据，并更新到巷道中
static void ReadTunnelFromMVSSData( const MVSSDataArray& td, MVSSDataLinkArray& datas )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        TunnelDataLink* pDL = ( TunnelDataLink* )datas[i];
        TunnelData* pTD = ( TunnelData* )td[i];

        acutPrintf( _T( "\nlink id:%d, data id:%d" ), pDL->objId, pTD->objId );

        DATA_COPY( pDL, pTD, name );
        DATA_COPY( pDL, pTD, q );
        DATA_COPY( pDL, pTD, r );
        DATA_COPY( pDL, pTD, lr );
        DATA_COPY( pDL, pTD, h );
        DATA_COPY( pDL, pTD, fq );
        DATA_COPY( pDL, pTD, rho );
        DATA_COPY( pDL, pTD, ad );
        DATA_COPY( pDL, pTD, ah );
        DATA_COPY( pDL, pTD, ar );
        DATA_COPY( pDL, pTD, rho1 );
        DATA_COPY( pDL, pTD, rho2 );
        DATA_COPY( pDL, pTD, rho3 );
        DATA_COPY( pDL, pTD, rho4 );
        DATA_COPY( pDL, pTD, tq );
        DATA_COPY( pDL, pTD, th );
        DATA_COPY( pDL, pTD, l );
        DATA_COPY( pDL, pTD, apha );
        DATA_COPY( pDL, pTD, r100 );
        DATA_COPY( pDL, pTD, atype );
        DATA_COPY( pDL, pTD, area );
        DATA_COPY( pDL, pTD, c );
        DATA_COPY( pDL, pTD, st );
        DATA_COPY( pDL, pTD, maxV );
        DATA_COPY( pDL, pTD, tUse );
        DATA_COPY( pDL, pTD, tRegion );
        DATA_COPY( pDL, pTD, temp );
        DATA_COPY( pDL, pTD, humidity );
        DATA_COPY( pDL, pTD, O2 );
        DATA_COPY( pDL, pTD, CH4 );
        DATA_COPY( pDL, pTD, CO2 );
        DATA_COPY( pDL, pTD, CO );
        DATA_COPY( pDL, pTD, reliability );
        DATA_COPY( pDL, pTD, type );
        DATA_COPY( pDL, pTD, comment );
        DATA_COPY( pDL, pTD, eh );
    }
}

// 从MVSSData中读取数据，并更新到风门中
static void ReadGateFromMVSSData( const MVSSDataArray& gd, MVSSDataLinkArray& datas )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        GateDataLink* pDL = ( GateDataLink* )datas[i];
        GateData* pGD = ( GateData* )gd[i];

        DATA_COPY( pDL, pGD, name );
        DATA_COPY( pDL, pGD, rq );
        DATA_COPY( pDL, pGD, flow );
        DATA_COPY( pDL, pGD, er );
        DATA_COPY( pDL, pGD, ea );
        DATA_COPY( pDL, pGD, trq );
    }
}

// 从MVSSData中读取数据，并更新到风机中
static void ReadFanFromMVSSData( const MVSSDataArray& fd, MVSSDataLinkArray& datas )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        FanDataLink* pDL = ( FanDataLink* )datas[i];
        FanData* pFD = ( FanData* )fd[i];

        DATA_COPY( pDL, pFD, name );
        DATA_COPY( pDL, pFD, rs );
        DATA_COPY( pDL, pFD, q );
        DATA_COPY( pDL, pFD, p );
        DATA_COPY( pDL, pFD, power );
        DATA_COPY( pDL, pFD, efficiency );
        DATA_COPY( pDL, pFD, r );
        DATA_COPY( pDL, pFD, aHole );
        DATA_COPY( pDL, pFD, model );
        DATA_COPY( pDL, pFD, angle );
        DATA_COPY( pDL, pFD, rotateSpeed );
        DATA_COPY( pDL, pFD, fq );
        DATA_COPY( pDL, pFD, nq );
        DATA_COPY( pDL, pFD, lq );
        DATA_COPY( pDL, pFD, fsp );
        DATA_COPY( pDL, pFD, ffp );
        DATA_COPY( pDL, pFD, outVP );
        DATA_COPY( pDL, pFD, inVP );
        DATA_COPY( pDL, pFD, inSP );
        DATA_COPY( pDL, pFD, tSP );
        DATA_COPY( pDL, pFD, cPower );
        DATA_COPY( pDL, pFD, inPower );
        DATA_COPY( pDL, pFD, outPower );
        DATA_COPY( pDL, pFD, nPower );
        DATA_COPY( pDL, pFD, lPower );
        DATA_COPY( pDL, pFD, uPower );
        DATA_COPY( pDL, pFD, ePower );
        DATA_COPY( pDL, pFD, sPower );
        DATA_COPY( pDL, pFD, fPower );
        DATA_COPY( pDL, pFD, fr );
        DATA_COPY( pDL, pFD, nr );
        DATA_COPY( pDL, pFD, lr );
        DATA_COPY( pDL, pFD, fma );
        DATA_COPY( pDL, pFD, fmv );
        DATA_COPY( pDL, pFD, outArea );
        DATA_COPY( pDL, pFD, outV );
        DATA_COPY( pDL, pFD, iq );
        DATA_COPY( pDL, pFD, comment );
        DATA_COPY( pDL, pFD, H_a0 );
        DATA_COPY( pDL, pFD, H_a1 );
        DATA_COPY( pDL, pFD, H_a2 );
        DATA_COPY( pDL, pFD, H_a3 );
        DATA_COPY( pDL, pFD, H_a4 );
        DATA_COPY( pDL, pFD, H_a5 );
        DATA_COPY( pDL, pFD, N_a0 );
        DATA_COPY( pDL, pFD, N_a1 );
        DATA_COPY( pDL, pFD, N_a2 );
        DATA_COPY( pDL, pFD, N_a3 );
        DATA_COPY( pDL, pFD, N_a4 );
        DATA_COPY( pDL, pFD, N_a5 );
        DATA_COPY( pDL, pFD, E_a0 );
        DATA_COPY( pDL, pFD, E_a1 );
        DATA_COPY( pDL, pFD, E_a2 );
        DATA_COPY( pDL, pFD, E_a3 );
        DATA_COPY( pDL, pFD, E_a4 );
        DATA_COPY( pDL, pFD, E_a5 );
    }
}

// 创建4个表
static void CreateMVSSTable( TableCreator* pTableCreator )
{
    JointData jd;
    jd.createTable( pTableCreator );

    TunnelData td;
    td.createTable( pTableCreator );

    GateData gd;
    gd.createTable( pTableCreator );

    FanData fd;
    fd.createTable( pTableCreator );
}

// 将MVSSData写入到数据库表中
static void WriteDatasToMVSSTable( MVSSDataArray& datas, DataWriter* pDataWriter )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        //acutPrintf(_T("\n写入对象id:%d"), datas[i]->objId);
        datas[i]->writeToTable( pDataWriter );
    }
}

static CDaoDatabase* CreateDB( const CString& dbName, bool openExist = false )
{
    CDaoDatabase* pDB = new CDaoDatabase();
    CFileFind ff;
    if( ff.FindFile( dbName ) )
    {
        if( openExist )
        {
            pDB->Open( dbName );
        }
        else
        {
            CFile::Remove( dbName );
            pDB->Create( dbName );
        }
    }
    else
    {
        pDB->Create( dbName );
    }
    return pDB;
}

// 将所有的数据写入到数据库中
static void WritAllDatasToMVSSDB( const CString& dbPath,
                                  MVSSDataArray& td,
                                  MVSSDataArray& jd,
                                  MVSSDataArray& gd,
                                  MVSSDataArray& fd )
{
    // 始终重建数据库(删除已存在的数据库)
    CDaoDatabase* pDB = CreateDB( dbPath, false );

    TableCreator* pTableCreator = new TableCreator( pDB );
    CreateMVSSTable( pTableCreator );
    delete pTableCreator;

    DataWriter* pDataWriter = new DataWriter( pDB );
    WriteDatasToMVSSTable( jd, pDataWriter );
    WriteDatasToMVSSTable( td, pDataWriter );
    WriteDatasToMVSSTable( gd, pDataWriter );
    WriteDatasToMVSSTable( fd, pDataWriter );
    delete pDataWriter;

    pDB->Close();
    delete pDB;
}

// 删除所有的MVSSData
static void ClearMVSSDatas( MVSSDataArray& datas )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        delete datas[i];
    }
    datas.removeAll();
}

// 删除所有MVSSData数据
static void ClearAllMVSSDatas( MVSSDataArray& td,
                               MVSSDataArray& jd,
                               MVSSDataArray& gd,
                               MVSSDataArray& fd )
{
    ClearMVSSDatas( jd );
    ClearMVSSDatas( td );
    ClearMVSSDatas( gd );
    ClearMVSSDatas( fd );
}

// 从数据库表中读取数据到MVSSData中
static void ReadDataFromMVSSDB( DataReader* pDataReader, const CString& name, MVSSDataArray& td )
{
    if( td.isEmpty() ) return;

    int i = 0;
    int len = td.length();

    pDataReader->beginReadData( name );
    while( i < len && pDataReader->hasNext() )
    {
        //acutPrintf(_T("\n读取%s数据，对象标识:%d"), name, objId);
        td[i]->readFromTable( pDataReader );

        i++;
        pDataReader->moveNext();
    }
    pDataReader->endReadData();
}

// 读取数据库的数据到MVSSData中
static void ReadAllDatasFromMVSSDB( const CString& dbPath,
                                    MVSSDataArray& td,
                                    MVSSDataArray& jd,
                                    MVSSDataArray& gd,
                                    MVSSDataArray& fd )
{
    CDaoDatabase db;
    db.Open( dbPath );
    DataReader* pDataReader = new DataReader( &db );

    ReadDataFromMVSSDB( pDataReader, JOINT_TABLE_NAME, jd );
    ReadDataFromMVSSDB( pDataReader, TUNNEL_TABLE_NAME, td );
    ReadDataFromMVSSDB( pDataReader, GATE_TABLE_NAME, gd );
    ReadDataFromMVSSDB( pDataReader, FAN_TABLE_NAME, fd );

    delete pDataReader;
    db.Close();
}

// 将DataLink中的数据更新到图元中
static void UpdateData( MVSSDataLinkArray& datas )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        datas[i]->updateData( true );
    }
}

static void TempTestData( MVSSDataArray& td )
{
    int len = td.length();
    for( int i = 0; i < len; i++ )
    {
        TunnelData* pTD = ( TunnelData* )td[i];
        CString str;
        str.Format( _T( "%d" ), ( i + 1 ) * 22 );
        pTD->q = str;
    }
}

// 宽字符转换成多字节
static BOOL WCharToMByte( const wchar_t* wch, char*& ch )
{
    int size = WideCharToMultiByte( CP_ACP, 0, wch, -1, NULL, 0, NULL, NULL );
    /*char* */
    ch = new char[size + 1];
    BOOL ret = WideCharToMultiByte( CP_ACP, 0, wch, -1, ch, size, NULL, NULL );
    if( !ret )
    {
        delete [] ch;
        ch = 0;
    }
    return ret;
}

// 调用mvss网络解算模块
static bool DoMVSS_VNC_Helper( const CString& dbPath )
{
    char* ch;
    if( !WCharToMByte( ( LPCTSTR )dbPath, ch ) ) return false;;

    CADCalNetwork o;
    bool ret = o.CalNetwork( ch );
    delete [] ch; // 删除动态分配的字符串数组内存

    return ret;
}

static void ClearDataLinkArray( MVSSDataLinkArray& datas )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        delete datas[i];
    }
    datas.removeAll();
}

MVSSVentNetworkCalc::MVSSVentNetworkCalc() : bInitNetwokState( true ), om( dg ), ngm( dg )
{
    // 初始化网络
    if( !( initNetwok() ) )
    {
        clear();
        setInitNetwokState( false );
    }
}

MVSSVentNetworkCalc::~MVSSVentNetworkCalc()
{
    if( bInitNetwokState )
    {
        //UpdateData(m_JDs);
        UpdateData( m_TDs );
        UpdateData( m_GDs );
        UpdateData( m_FDs );
        clear();
    }
    /*
    * 还应该有一些其它的后续工作
    * 1、将负值风量修改成正值
    * 2、改变巷道上所有的方向类图元的方向(风流方向，风机方向等)
    * 3、计算固定风量分支的调节风阻
    */
}

bool MVSSVentNetworkCalc::initNetwok()
{
    // 创建通风网络
    if( !BuildNetwork( dg, om ) )
    {
        acutPrintf( _T( "\n创建网络失败" ) );
        return false;
    }

    // 获取节点坐标
    if( !GetNodeGeoData( dg, om, ngm ) )
    {
        acutPrintf( _T( "\n获取节点坐标失败" ) );
        return false;
    }

    if( !CreateTunnelDataLinks( dg, om, m_TDs ) )
    {
        acutPrintf( _T( "\n提取巷道数据失败" ) );
        return false;
    }
    if( !CreateJointDataLinks( dg, ngm, m_JDs ) )
    {
        acutPrintf( _T( "\n提取节点数据失败" ) );
        return false;
    }
    if( !CreateGateDataLinks( dg, om, m_GDs ) )
    {
        acutPrintf( _T( "\n提取风门构筑物数据失败" ) );
        return false;
    }
    if( !CreateMainFanDataLinks( dg, om, m_FDs ) )
    {
        acutPrintf( _T( "\n提取主要通风机数据失败" ) );
        return false;
    }
    if( !CreateLocalFanDataLinks( dg, om, ngm, m_TDs, m_FDs ) )
    {
        acutPrintf( _T( "\n提取局部通风机数据失败" ) );
        return false;
    }

    return true;
}

bool MVSSVentNetworkCalc::isInitNetworkOk() const
{
    return bInitNetwokState;
}

void MVSSVentNetworkCalc::setInitNetwokState( bool bState )
{
    bInitNetwokState = bState;
}

void MVSSVentNetworkCalc::clear()
{
    ClearDataLinkArray( m_TDs );
    ClearDataLinkArray( m_JDs );
    ClearDataLinkArray( m_GDs );
    ClearDataLinkArray( m_FDs );
}

bool MVSSVentNetworkCalc::doVNC()
{
    CString dbFile = _T( "D:\\test.mdb" );
    bool ret = doMVSS_VNC( dbFile ); // 使用mvss进行网络解算
    //RemoveFile(dbFile);            // 删除数据库文件
    return ret;
}

// 进行网络解算
bool MVSSVentNetworkCalc::doMVSS_VNC( const CString& dbPath )
{
    if( !isInitNetworkOk() ) return false;

    // 填充数据
    MVSSDataArray td, jd, gd, fd;
    CreateTunnelData( m_TDs, td );
    CreateJointData( m_JDs, jd );
    CreateGateData( m_GDs, gd );
    CreateFanData( m_FDs, fd );

    // 写入数据到数据库中
    WritAllDatasToMVSSDB( dbPath, td, jd, gd, fd );

    // 调用mvss网络解算模块
    if( DoMVSS_VNC_Helper( dbPath ) )
    {
        acutPrintf( _T( "\n调用mvss网络解算模块成功!!!" ) );

        /*
         * 解算成功!!!
         * 从数据库中将数据读回来
         */
        ReadAllDatasFromMVSSDB( dbPath, td, jd, gd, fd );

        // 临时测试代码
        // 向JointData中随意添加一些数据
        //TempTestData(td);

        // 将数据写入到DataLink中
        ReadTunnelFromMVSSData( td, m_TDs );
        ReadJointFromMVSSData( jd, m_JDs );
        ReadGateFromMVSSData( gd, m_GDs );
        ReadFanFromMVSSData( fd, m_FDs );
    }

    // 清理
    ClearAllMVSSDatas( td, jd, gd, fd );

    return true;
}

JointDataLink* MVSSVentNetworkCalc::getJointData( const AcGePoint3d& pt )
{
    if( !isInitNetworkOk() ) return 0;

    JointDataLink* pJDL = 0;
    int len = m_JDs.length();
    for( int i = 0; i < len; i++ )
    {
        pJDL = ( JointDataLink* )m_JDs[i];
        AcGePoint3d ppt( pJDL->x, pJDL->y, pJDL->z );
        if( pt == ppt )
        {
            break;
        }
    }
    return pJDL;
}
