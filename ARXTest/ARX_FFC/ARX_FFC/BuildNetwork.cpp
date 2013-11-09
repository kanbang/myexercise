#include "stdafx.h"
#include "BuildNetwork.h"
#include "../MineGE/LinkedGE.h"

static void GetEdgeSEPts( const AcDbObjectIdArray& objIds, AcGePoint3dArray& spts, AcGePoint3dArray& epts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 )
    {
        //acutPrintf(_T("\n����transactionʧ��"));
        return;
    }

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) )
        {
            //acutPrintf(_T("�򿪵�%d����֧ͼԪʧ��"), i);
            continue;
        }
        LinkedGE* pEdge = LinkedGE::cast( pObj );

        //acutPrintf(_T("\n��ȡ��%d����֧ͼԪ"), i);
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

bool BuildNetwork( const AcDbObjectIdArray& objIds, Digraph& dg, ObjectIdMap& om )
{
    // 2) ��ȡ���еķ�֧ͼԪ��ʼĩ������
    AcGePoint3dArray spts, epts;
    GetEdgeSEPts( objIds, spts, epts );
    //acutPrintf(_T("\n��֧�������:%d"), spts.length());
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

    // 6) �����֧����idӳ��om
    int len = edges.length();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = edges[i];
        om[e] = objIds[i];
    }

    //acutPrintf(_T("\nԭʼͼ�ڵ����:%d ��֧����:%d"), countNodes(dg), countArcs(dg));

    return true;
}