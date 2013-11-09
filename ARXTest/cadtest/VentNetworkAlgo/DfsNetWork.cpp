#include "StdAfx.h"
#include "DfsNetWork.h"

#include "../MineGE/ConstData.h"

#include "../ArxHelper/ArxUtilHelper.h"
#include "../ArxHelper/ArxEntityHelper.h"

#include <lemon/adaptors.h>
#include <lemon/dfs.h>

#include "../GraphTool/Tool.h"

static void ClearGraph( Digraph& dg, DFS_EdgeDataMap& datas )
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

static void InitEdgeDatas( Digraph& dg, ObjectIdMap& obj_map, DFS_EdgeDataMap& datas )
{
    // ������֧����
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        EdgeData* pEdgeData = new EdgeData();
        pEdgeData->objId = obj_map[e];
        SetEdgeType( pEdgeData->objId, pEdgeData );
        datas[e] = pEdgeData; // ����һ��Digraph::Arc����
    }
}

DfsNetWork::DfsNetWork( void ) : datas( dg ), ef( dg, true ), bInitNetwokState( true )
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

DfsNetWork::~DfsNetWork( void )
{
    if( bInitNetwokState )
    {
        ClearGraph( dg, datas );
    }
}

bool DfsNetWork::initNetwok()
{
    ObjectIdMap om( dg );
    if( !BuildNetwork( dg, om ) ) return false;
    if( !AddVirtualSTNode( dg, om, sn, tn ) ) return false;
    FilterBlockEdges( dg, om, ef );

    // ������֧��������
    InitEdgeDatas( dg, om, datas );

    return true;
}

bool DfsNetWork::isInitNetworkOk() const
{
    return bInitNetwokState;
}

void DfsNetWork::setInitNetwokState( bool bState )
{
    bInitNetwokState = bState;
}

void DfsNetWork::initAirEdges()
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( datas[e]->et == ET_USEAIR )
        {
            airEdges.append( e );
        }
    }
}

Digraph::Arc DfsNetWork::getEdgeById( const AcDbObjectId& objId )
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

AcDbObjectId DfsNetWork::getIdByEdge( Digraph::Arc e )
{
    if( e == INVALID ) return AcDbObjectId::kNull;
    // Ӧ���жϷ�֧�Ƿ������ͼ��
    // Ŀǰ�ͼ���e�϶���ͼ�ķ�֧
    return datas[e]->objId;
}

void DfsNetWork::doDFS( Digraph::Arc se, Digraph::Arc te, EdgeArray& p )
{
    if( se == INVALID || te == INVALID ) return;

    Digraph::Node s = dg.source( se );
    Digraph::Node t = dg.target( te );

    Dfs<Digraph> aDfs( dg );
    if( aDfs.run( s, t ) )
    {
        typedef Path<Digraph> DPath;
        DPath dp = aDfs.path( t );

        for( DPath::ArcIt e( dp ); e != INVALID; ++e )
        {
            p.append( e );
        }
    }
}