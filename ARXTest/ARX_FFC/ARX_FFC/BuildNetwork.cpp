#include "stdafx.h"
#include "BuildNetwork.h"
#include "../MineGE/LinkedGE.h"

static void GetEdgeSEPts( const AcDbObjectIdArray& objIds, AcGePoint3dArray& spts, AcGePoint3dArray& epts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 )
    {
        //acutPrintf(_T("\n启动transaction失败"));
        return;
    }

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) )
        {
            //acutPrintf(_T("打开第%d个分支图元失败"), i);
            continue;
        }
        LinkedGE* pEdge = LinkedGE::cast( pObj );

        //acutPrintf(_T("\n读取第%d个分支图元"), i);
        AcGePoint3d spt, ept;
        pEdge->getSEPoint( spt, ept );

        // 记录始末点坐标
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
    // 建立点坐标和节点之间的映射
    int len = pts.length();
    for( int i = 0; i < len; i++ )
    {
        nodes.append( dg.addNode() );
    }
}

static void BuildEdgesMap( const AcDbObjectIdArray& objIds, const AcGePoint3dArray& pts, Digraph& dg, NodeArray& nodes, EdgeArray& edges )
{
    // 添加分支
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

bool BuildNetwork( const AcDbObjectIdArray& objIds, Digraph& dg, ObjectIdMap& om )
{
    // 2) 获取所有的分支图元的始末点坐标
    AcGePoint3dArray spts, epts;
    GetEdgeSEPts( objIds, spts, epts );
    //acutPrintf(_T("\n分支坐标个数:%d"), spts.length());
    if( spts.length() != objIds.length() ) return false;

    // 分析始末点坐标，获取节点坐标
    AcGePoint3dArray pts;
    GetNodePts( spts, epts, pts );

    // 3) 建立点坐标和节点的映射
    NodeArray nodes;
    BuildNodesMap( pts, dg, nodes );

    // 4) 建立图元和分支的映射
    EdgeArray edges;
    BuildEdgesMap( objIds, pts, dg, nodes, edges );

    // 6) 构造分支对象id映射om
    int len = edges.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = edges[i];
        om[e] = objIds[i];
    }

    //acutPrintf(_T("\n原始图节点个数:%d 分支个数:%d"), countNodes(dg), countArcs(dg));

    return true;
}