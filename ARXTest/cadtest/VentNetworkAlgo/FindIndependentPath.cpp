#include "StdAfx.h"
#include "FindIndependentPath.h"

#include "../ArxHelper/ArxUtilHelper.h"
#include "../ArxHelper/ArxEntityHelper.h"

#include <lemon/adaptors.h>
#include <lemon/dfs.h>

typedef FilterArcs<Digraph, ArcFilter> AFGraph;

//#include "../Tool/MyRandom.h"
#include "../Tool/Timer.h"

#include "../GraphTool/Tool.h"

static void ClearGraph( Digraph& dg, FIP_EdgeDataMap& datas )
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
static void SetEdgeType( const AcDbObjectId& objId, EdgeData* pEdgeData )
{
    if( objId.isNull() )
    {
        pEdgeData->et = ET_VIRTUAL; // �����֧
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
            if( IsMainPowerSubtation( objId ) )
            {
                pEdgeData->et = ET_COMMON;  // ��������
            }
            else
            {
                pEdgeData->et = ET_USEAIR;  // ���������(�÷�ص�)
            }
        }
        else
        {
            pEdgeData->et = ET_COMMON;
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
}

static void InitEdgeDatas( Digraph& dg, ObjectIdMap& om, FIP_EdgeDataMap& datas )
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

static bool DFS_Helper( AFGraph& afg, Digraph::Node u, Digraph::Node v, EdgeArray& p )
{
    // ִ��dfs
    Dfs<AFGraph> aDfs( afg );
    bool ret = aDfs.run( u, v );
    if( ret )
    {
        typedef Path<AFGraph> DPath;
        DPath dp = aDfs.path( v ); // u->v

        for( DPath::ArcIt e( dp ); e != INVALID; ++e )
        {
            p.append( e );
        }
    }
    return ret;
}

// ֻҪ����һ��·������
static bool FIP( AFGraph& afg, EdgeArray& airEdges, Digraph::Node s, Digraph::Node t, EdgeArray& p )
{
    bool ret = false;
    int len = airEdges.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc ae = airEdges[i];

        Digraph::Node u = afg.source( ae );
        Digraph::Node v = afg.target( ae );

        if( t == u || s == v ) continue;

        EdgeArray tp1, tp2; // ��ʱ·��
        bool ret1 = DFS_Helper( afg, s, u, tp1 );
        bool ret2 = DFS_Helper( afg, v, t, tp2 );
        if( ret1 && ret2 )
        {
            p.append( tp1 );
            p.append( ae );
            p.append( tp2 );
            ret = true;
            break;
        }
    }
    // û���÷�ص����û���ҵ��κ�һ������·��
    // �ٳ���u->v֮���һ��·��
    if( !ret )
    {
        EdgeArray tp;
        ret = DFS_Helper( afg, s, t, tp );
        if( ret )
        {
            p.append( tp );
        }
    }
    return ret;
}

static bool FIP_Helper( AFGraph& afg, EdgeArray& airEdges,
                        Digraph::Node sn, Digraph::Node tn,
                        Digraph::Node u, Digraph::Node v,
                        EdgeArray& p1, EdgeArray& p2 )
{
    // sn->u
    bool ret = FIP( afg, airEdges, sn, u, p1 );
    if( ret )
    {
        // v->tn
        ret = FIP( afg, airEdges, v, tn, p2 );
    }
    return ret;
}

FindIndependentPath::FindIndependentPath( void ) : datas( dg ), ef( dg, true ), bInitNetwokState( true )
{
    // ��ʼ�������Լ���Ӵ���Դ��
    if( !initNetwok() )
    {
        ClearGraph( dg, datas );
        setInitNetwokState( false );
    }
    else
    {
        initAirEdges();   // ��¼�÷�ص��֧
    }
}

FindIndependentPath::~FindIndependentPath( void )
{
    if( bInitNetwokState )
    {
        // ����Ҫ��������
//		UpdateData(dg, datas);
        ClearGraph( dg, datas );
    }
}

Digraph::Arc FindIndependentPath::getEdgeById( const AcDbObjectId& objId )
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

AcDbObjectId FindIndependentPath::getIdByEdge( Digraph::Arc e )
{
    if( e == INVALID ) return AcDbObjectId::kNull;
    // Ӧ���жϷ�֧�Ƿ������ͼ��
    // Ŀǰ�ͼ���e�϶���ͼ�ķ�֧
    return datas[e]->objId;
}

void FindIndependentPath::findAirEdges( AcDbObjectIdArray& objIds )
{
    int len = airEdges.length();
    for( int i = 0; i < len; i++ )
    {
        objIds.append( getIdByEdge( airEdges[i] ) );
    }
}

bool FindIndependentPath::doFip( Digraph::Arc e, EdgeArray& p1, EdgeArray& p2 )
{
    if( !isInitNetworkOk() ) return false;
    if( e == INVALID ) return false;

    // ������Ϸ�֧���ͼ
    AFGraph afg( dg, ef );

    double st = Timer::rdtscSeconds();

    //acutPrintf(_T("\n��ʼִ�з�֧dofip()����"));
    EdgeArray pp1, pp2;
    bool ret = FIP_Helper( afg, airEdges, sn, tn, dg.source( e ), dg.target( e ), pp1, pp2 );
    if( ret )
    {
        //acutPrintf(_T("\n·�������ɹ�"));
        int len = pp1.length();
        for( int i = 0; i < len; i++ )
        {
            Digraph::Arc e = pp1[i];
            if( datas[e]->et != ET_VIRTUAL ) p1.append( e );
        }
        len = pp2.length();
        for( int i = 0; i < len; i++ )
        {
            Digraph::Arc e = pp2[i];
            if( datas[e]->et != ET_VIRTUAL ) p2.append( e );
        }
        //acutPrintf(_T("\n����ͨ·���ȣ�%d\t�ط�ͨ·����:%d"), p1.length(), p2.length());
    }

    double et = Timer::rdtscSeconds();
    acutPrintf( _T( "\ndoFip�ķ�ʱ��%.5f" ), et - st );

    return ret;
}

bool FindIndependentPath::initNetwok()
{
    // ��������
    ObjectIdMap om( dg );
    if( !BuildNetwork( dg, om ) ) return false;
    if( !AddVirtualSTNode( dg, om, sn, tn ) ) return false;
    FilterBlockEdges( dg, om, ef );

    // ������֧��������
    InitEdgeDatas( dg, om, datas );

    return true;
}

bool FindIndependentPath::isInitNetworkOk() const
{
    return bInitNetwokState;
}

void FindIndependentPath::setInitNetwokState( bool bState )
{
    bInitNetwokState = bState;
}

void FindIndependentPath::initAirEdges()
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( datas[e]->et == ET_USEAIR )
        {
            airEdges.append( e );
        }
    }
}

bool FindIndependentPath::isIncludeAirEdge( EdgeArray& p )
{
    bool ret = false;
    int len = p.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = p[i];
        if( datas[e]->et == ET_USEAIR )
        {
            // ����Ƿ��÷��֧
            ret = true;
            break;
        }
    }
    return ret;
}

void FindIndependentPath::findSerialEdges( EdgeArray& p, EdgeArray& se )
{
    int len = p.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = p[i];
        if( datas[e]->et == ET_USEAIR )
        {
            se.append( e );
        }
    }
}

void FindIndependentPath::displayPath( EdgeArray& p, unsigned short colorIndex )
{
    AcDbObjectIdArray objIds;
    int len = p.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = p[i];
        objIds.append( datas[e]->objId );
    }
    ArxEntityHelper::SetEntitiesColor( objIds, colorIndex );
}
