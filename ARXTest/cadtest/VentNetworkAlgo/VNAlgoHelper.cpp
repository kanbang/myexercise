#include "StdAfx.h"
#include "VNAlgoHelper.h"

#include "FindIndependentPath.h"
#include "SetWindDir.h"
#include "IdentifyLiaisonTunnel.h"
#include "ConnectivityVote.h"
#include "FindWindStation.h"

bool VNAlgoHelper::DoSWD()
{
    SetWindDir swd;
    return swd.doSWD();
}

ILTHelper::ILTHelper()
{
    pILT = new IdentifyLiaisonTunnel();
}

ILTHelper::~ILTHelper()
{
    delete pILT;
}

bool ILTHelper::doILT( AcDbObjectIdArray& objIds )
{
    return doILT3( objIds );
}

bool ILTHelper::doILT2( AcDbObjectIdArray& objIds )
{
    EdgeArray edges;
    if( !pILT->doILT2( edges ) ) return false;
    //if(!pILT->doILT4(edges)) return false;

    int len = edges.length();
    for( int i = 0; i < len; i++ )
    {
        objIds.append( pILT->getIdByEdge( edges[i] ) );
    }
    return true;
}

bool ILTHelper::doILT3( AcDbObjectIdArray& objIds )
{
    EdgeArray edges;
    if( !pILT->doILT3( edges ) ) return false;

    int len = edges.length();
    for( int i = 0; i < len; i++ )
    {
        objIds.append( pILT->getIdByEdge( edges[i] ) );
    }
    return true;
}

bool ILTHelper::findInOutEdges( const AcDbObjectId& objId, AcDbObjectIdArray& s1, AcDbObjectIdArray& s2 )
{
    EdgeArray es1, es2;
    if( !pILT->findInOutEdges( pILT->getEdgeById( objId ), es1, es2 ) ) return false;

    int len = es1.length();
    for( int i = 0; i < len; i++ )
    {
        s1.append( pILT->getIdByEdge( es1[i] ) );
    }
    len = es2.length();
    for( int i = 0; i < len; i++ )
    {
        s2.append( pILT->getIdByEdge( es2[i] ) );
    }
    return true;
}

FIPHelper::FIPHelper()
{
    pFIP = new FindIndependentPath();
}

FIPHelper::~FIPHelper()
{
    delete pFIP;
}

bool FIPHelper::doFIP( const AcDbObjectId& objId, AcDbObjectIdArray& p1, AcDbObjectIdArray& p2 )
{
    EdgeArray es_p1, es_p2;
    if( !pFIP->doFip( pFIP->getEdgeById( objId ), es_p1, es_p2 ) ) // 搜索失败
    {
        /*
         * 有多个原因
         *		1) 所选图元不是分支
         *		2) 所选择的图元与源汇之间不可达
         */
        acutPrintf( _T( "\n未知错误~~~" ) );
        return false;
    }

    //bool ret = !(pFIP->isIncludeAirEdge(es_p1) && pFIP->isIncludeAirEdge(es_p2));

    int len = es_p1.length();
    for( int i = 0; i < len; i++ )
    {
        p1.append( pFIP->getIdByEdge( es_p1[i] ) );
    }
    len = es_p2.length();
    for( int i = 0; i < len; i++ )
    {
        p2.append( pFIP->getIdByEdge( es_p2[i] ) );
    }
    return true;
}

void FIPHelper::findAirEdges( AcDbObjectIdArray& objIds )
{
    pFIP->findAirEdges( objIds );
}

void FIPHelper::findSerialEdges( AcDbObjectIdArray& p, AcDbObjectIdArray& se )
{
    EdgeArray pp;
    int len = p.length();
    for( int i = 0; i < len; i++ )
    {
        pp.append( pFIP->getEdgeById( p[i] ) );
    }

    EdgeArray es;
    pFIP->findSerialEdges( pp, es );

    len = es.length();
    for( int i = 0; i < len; i++ )
    {
        se.append( pFIP->getIdByEdge( es[i] ) );
    }
}

bool FIPHelper::isIncludeAirEdge( const AcDbObjectIdArray& p )
{
    EdgeArray es;
    int len = p.length();
    for( int i = 0; i < len; i++ )
    {
        es.append( pFIP->getEdgeById( p[i] ) );
    }

    return pFIP->isIncludeAirEdge( es );
}

bool ConnectivityHelper::Connected()
{
    ConnectivityVote cnn;
    return cnn.connected();
}

bool ConnectivityHelper::DAG()
{
    ConnectivityVote cnn;
    return cnn.dag();
}

bool ConnectivityHelper::CC( AcDbObjectIdArray& objIds, AcDbIntArray& ccIds )
{
    ConnectivityVote cnn;
    return cnn.CC( objIds, ccIds );
}

bool ConnectivityHelper::SCC( AcDbObjectIdArray& objIds, AcDbIntArray& sccIds )
{
    ConnectivityVote cnn;
    return cnn.SCC( objIds, sccIds );
}

void ConnectivityHelper::FIE( AcDbObjectIdArray& objIds )
{
    ConnectivityVote cnn;
    cnn.FIE( objIds );
}

void ConnectivityHelper::FOE( AcDbObjectIdArray& objIds )
{
    ConnectivityVote cnn;
    cnn.FOE( objIds );
}

void ConnectivityHelper::FIOE( AcDbObjectIdArray& objIds1, AcDbObjectIdArray& objIds2 )
{
    ConnectivityVote cnn;
    cnn.FIE( objIds1 );
    cnn.FOE( objIds2 );
}

FWSHelper::FWSHelper()
{
    pFWS = new FindWindStation();
}

FWSHelper::~FWSHelper()
{
    delete pFWS;
    pFWS = 0;
}

AcDbObjectId FWSHelper::doFWS( const AcDbObjectId& objId )
{
    Digraph::Arc e = pFWS->doFWS( pFWS->getEdgeById( objId ) );
    return pFWS->getIdByEdge( e );
}
