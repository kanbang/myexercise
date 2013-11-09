#include "StdAfx.h"
#include "IntFlow.h"

#include "../ArxHelper/ArxUtilHelper.h"
#include "../ArxHelper/ArxEntityHelper.h"

#include "../GraphTool/Tool.h"

/* ȫ�ֺ���(ʵ����DisplayIntValue.cpp��) */
extern void DisplayIntValue( const AcDbObjectIdArray& objIds, const AcDbIntArray& ivs );

static void DisplayIntValue( Digraph& dg, ArcIntFlowMap& datas )
{
    AcDbObjectIdArray objIds;
    AcDbIntArray ivs;
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        objIds.append( datas[e]->objId );
        ivs.append( datas[e]->iv );
    }

    DisplayIntValue( objIds, ivs );
}

static void ClearGraph( Digraph& dg, ArcIntFlowMap& datas )
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
static void SetEdgeType( const AcDbObjectId& objId, ArcIntFlowData* pEdgeData )
{
    if( objId.isNull() )
    {
        pEdgeData->et = ET_VIRTUAL;
    }
    if( ArxUtilHelper::IsEqualType( _T( "WorkSurface" ), objId ) )
    {
        pEdgeData->et = ET_USEAIR;  // ������
    }
    else if( ArxUtilHelper::IsEqualType( _T( "StorageGE" ), objId ) )
    {
        if( IsPowerSubstation( objId ) )
        {
            pEdgeData->et = ET_USEAIR;  // �����
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

static void InitEdgeDatas( Digraph& dg, ObjectIdMap& om, ArcIntFlowMap& datas )
{
    // ������֧����
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        ArcIntFlowData* pEdgeData = new ArcIntFlowData();
        pEdgeData->objId = om[e];
        SetEdgeType( pEdgeData->objId, pEdgeData );
        datas[e] = pEdgeData; // ����һ��Digraph::Arc����
    }
}

IntFlow::IntFlow( void ) : datas( dg ), ef( dg, true ), bInitNetwokState( true )
{
    // ��ʼ�������Լ���Ӵ���Դ��
    if( !( initNetwok() ) )
    {
        ClearGraph( dg, datas );
        setInitNetwokState( false );
    }
    else
    {
        initAirEdges();   // ��¼�÷�ص��֧
    }
}

IntFlow::~IntFlow( void )
{
    if( bInitNetwokState )
    {
        ClearGraph( dg, datas );
    }
}

Digraph::Arc IntFlow::getEdgeById( const AcDbObjectId& objId )
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

AcDbObjectId IntFlow::getIdByEdge( Digraph::Arc e )
{
    if( e == INVALID ) return AcDbObjectId::kNull;
    // Ӧ���жϷ�֧�Ƿ������ͼ��
    // Ŀǰ�ͼ���e�϶���ͼ�ķ�֧
    return datas[e]->objId;
}

bool IntFlow::initNetwok()
{
    // ��������
    ObjectIdMap om( dg );
    if( !BuildNetwork( dg, om ) ) return false;
    FilterBlockEdges( dg, om, ef );
    if( !AddVirtualSTNode( dg, om, sn, tn ) ) return false;

    // ������֧��������
    InitEdgeDatas( dg, om, datas );

    return true;
}

bool IntFlow::isInitNetworkOk() const
{
    return bInitNetwokState;
}

void IntFlow::setInitNetwokState( bool bState )
{
    bInitNetwokState = bState;
}

void IntFlow::initAirEdges()
{
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( datas[e]->et == ET_USEAIR )
        {
            airEdges.append( e );
        }
    }
}

// �����������-����������(�ų���֧e)
int IntFlow::caclEdgeOutFlow( Digraph::Arc e )
{
    Digraph::Node u = dg.source( e );

    // ���������
    int iq = 0;
    for( Digraph::InArcIt ie( dg, u ); ie != INVALID; ++ie )
    {
        iq += ( datas[ie]->iv );
    }

    // ��������(�ų���֧e)
    int oq = 0;
    for( Digraph::OutArcIt oe( dg, u ); oe != INVALID; ++oe )
    {
        if( oe == e ) continue;
        oq += ( datas[oe]->iv );
    }

    return ( iq - oq );
}

// ������������-���������(�ų���֧e)
int IntFlow::caclEdgeInFlow( Digraph::Arc e )
{
    Digraph::Node u = dg.target( e );

    // ����������
    int oq = 0;
    for( Digraph::OutArcIt oe( dg, u ); oe != INVALID; ++oe )
    {
        oq += ( datas[oe]->iv );
    }

    // �������(�ų���֧e)
    int iq = 0;
    for( Digraph::InArcIt ie( dg, u ); ie != INVALID; ++ie )
    {
        if( ie == e ) continue;
        iq += ( datas[ie]->iv );
    }

    return ( oq - iq );
}

void IntFlow::initEdgeFlow()
{
    acutPrintf( _T( "\n��ʼ��..." ) );
    // ��ʼ�����з�֧iv=1
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        datas[e]->iv = 1;
        //acutPrintf(_T("\n��֧:e%d\tiv=%d"), dg.id(e), datas[e]->iv);
    }
}

void IntFlow::doCacl()
{
    initEdgeFlow();

    // �������(Ŀǰ���Թ���Ϊ��֧��)
    int maxQ = countArcs( dg );

    acutPrintf( _T( "\n��ʼ����..." ) );
    bool bQuit = false;
    int count = 0;
    while( !bQuit && count <= maxQ )
    {
        count++;
        bQuit = true;
        for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
        {
            int oq = caclEdgeOutFlow( e );
            int iq = caclEdgeInFlow( e );
            int mq = max( oq, iq );
            //acutPrintf(_T("\n��֧:e%d   iq=%d   oq=%d\t������:%d"), dg.id(e), iq, oq, mq);

            if( mq > datas[e]->iv )
            {
                bQuit = false;
                datas[e]->iv = mq;
            }
        }
        //acutPrintf(_T("\n===========================================\n"));
    }
    acutPrintf( _T( "\ncount=%d" ), count );

    // �����ã���ʾivalue
    DisplayIntValue( dg, datas );
}

