#include "StdAfx.h"

#include "VNGParam.h"

#include "../VNG_GE/VNG_Node.h"
#include "../VNG_GE/VNG_Edge.h"
#include "../VNG_GE/VNG_Gate.h"
#include "../VNG_GE/VNG_Fan.h"

#include "../ArxHelper/HelperClass.h"

void UpdateAllNodesParam( NodeParam& np )
{
    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( _T( "VNG_Node" ), objIds );
    if( objIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();

    int n = objIds.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        VNG_Node* pNode = VNG_Node::cast( pObj );

        pNode->setParam( np.m_nodeWidth, np.m_nodeHeight, np.m_nodeTextHeight );

        pNode->recordGraphicsModified( true );
    }

    actrTransactionManager->endTransaction();
}

void UpdateAllEdgesParam( EdgeParam& ep )
{
    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( _T( "VNG_Edge" ), objIds );
    if( objIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();

    int n = objIds.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        VNG_Edge* pEdge = VNG_Edge::cast( pObj );
        pEdge->setParam( ep.m_arrowWidth, ep.m_arrowLength, ep.m_edgeTextHeight, ep.m_needEdge );
        pEdge->recordGraphicsModified( true );
    }

    actrTransactionManager->endTransaction();
}

void UpdateAllGatesParam( GateParam& gp )
{
    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( _T( "VNG_Gate" ), objIds );
    if( objIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();

    int n = objIds.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        VNG_Gate* pGate = VNG_Gate::cast( pObj );
        pGate->setParam( gp.m_radius, gp.m_length );
        pGate->recordGraphicsModified( true );
    }

    actrTransactionManager->endTransaction();
}

void UpdateAllFansParam( FanParam& fp )
{
    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( _T( "VNG_Fan" ), objIds );
    if( objIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();

    int n = objIds.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        VNG_Fan* pFan = VNG_Fan::cast( pObj );
        pFan->setParam( fp.m_radius, fp.m_distance );
        pFan->recordGraphicsModified( true );
    }

    actrTransactionManager->endTransaction();
}