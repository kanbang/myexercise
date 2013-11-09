#include "stdafx.h"
#include "DrawCmd.h"

#include "DragJig.h"
#include "JunctionEdgeInfo.h"

#include "../FFC_GE/Goaf.h"

#include "../MineGE/LinkedGE.h"
#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

extern void GetEdgeSEPts( const AcDbObjectIdArray& objIds, AcGePoint3dArray& spts, AcGePoint3dArray& epts );
extern bool GetSEPt( const AcDbObjectId& objId, AcGePoint3d& spt, AcGePoint3d& ept );

extern void AutoDirection();

// 查找所有关联的巷道
static void FindTunnelsByPoint( const AcGePoint3d& junctionPt, JunctionEdgeInfoArray& ges )
{
    // 查找所有的巷道
    AcDbObjectIdArray allObjIds;
    DrawHelper::FindMineGEs( _T( "Tunnel" ), allObjIds );

    AcGePoint3dArray spts, epts;
    GetEdgeSEPts( allObjIds, spts, epts );

    int n = spts.length();
    for( int i = 0; i < n; i++ )
    {
        if( spts[i] == junctionPt || epts[i] == junctionPt )
        {
            JunctionEdgeInfo info;
            info.objId = allObjIds[i];
            info.startOrEnd = ( spts[i] == junctionPt );
            info.angle = ( info.startOrEnd ? ( epts[i] - spts[i] ) : ( spts[i] - epts[i] ) );

            ges.append( info );
        }
    }
}

static void GetGoafPolygon( const AcDbObjectId& objId, AcGePoint3dArray& polygon )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk == pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        Goaf* pGoaf = Goaf::cast( pObj );
        if( pGoaf != 0 )
        {
            pGoaf->getPolygon( polygon );
        }
    }

    actrTransactionManager->endTransaction();
}

static int FindPointOnPolygon( const AcGePoint3d& pt, const AcGePoint3dArray& polygon )
{
    int pos = -1;
    int n = polygon.length();
    for( int i = 0; i < n; i++ )
    {
        AcGePoint3d spt = polygon[i], ept = polygon[( i + 1 ) % n];
        if( spt == pt )
        {
            pos = i;
            break;
        }
        else if( ept == pt )
        {
            pos = ( i + 1 ) % n;
            break;
        }
    }
    return pos;
}

static bool FindGoafByPoint( const AcGePoint3d& spt, const AcGePoint3d& ept, AcDbObjectId& objId, int& p1, int& p2 )
{
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "Goaf" ), objIds );
    if( objIds.isEmpty() ) return false;

    bool ret = false;
    for( int i = 0; i < objIds.length(); i++ )
    {
        // 获取采空区的多边形
        AcGePoint3dArray polygon;
        GetGoafPolygon( objIds[i], polygon );
        int n = polygon.length();

        objId = objIds[i];
        p1 = FindPointOnPolygon( spt, polygon );
        p2 = FindPointOnPolygon( ept, polygon );
        //acutPrintf(_T("\ni=%d p1=%d p2=%d"), i, p1, p2);
        if( p1 == -1 || p2 == -1 ) continue;
        if( abs( p1 - p2 ) == 1 || abs( p1 - p2 ) == n - 1 )
        {
            ret = true;
            break;
        }
    }
    return ret;
}

static void SetNewGoafPoint( const AcDbObjectId& objId, int pos, const AcGePoint3d& pt )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk == pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        Goaf* pGoaf = Goaf::cast( pObj );
        if( pGoaf != 0 )
        {
            pGoaf->setPoint( pos, pt );
        }
    }

    actrTransactionManager->endTransaction();
}

void DrawCmd::AdvanceWS()
{
    // 提示用户选择一个工作面
    AcDbObjectId ws_objId = ArxUtilHelper::SelectObject( _T( "\n请选择一个工作面" ) );
    if( ws_objId.isNull() ) return;

    // 获取工作面的始末坐标
    AcGePoint3d spt, ept;
    GetSEPt( ws_objId, spt, ept );

    // 查找关联的巷道
    JunctionEdgeInfoArray sptLinesInfo, eptLinesInfo;
    FindTunnelsByPoint( spt, sptLinesInfo );
    FindTunnelsByPoint( ept, eptLinesInfo );

    // 查找关联的采空区
    AcDbObjectId goaf_objId;
    int p1, p2;
    if( !FindGoafByPoint( spt, ept, goaf_objId, p1, p2 ) )
    {
        goaf_objId.setNull();
        p1 = p2 = -1;
    }

    // 执行jig拖拽
    DragJig jig( ws_objId, sptLinesInfo, eptLinesInfo, goaf_objId, p1, p2 );
    if( jig.doIt() )
    {
        // 更新所有的关联巷道的连接点闭合
        DrawHelper::LinkedGEJunctionClosure2( ws_objId );
        int m = sptLinesInfo.length();
        for( int i = 0; i < m; i++ )
        {
            DrawHelper::LinkedGEJunctionClosure2( sptLinesInfo[i].objId );
        }
        int n = eptLinesInfo.length();
        for( int i = 0; i < n; i++ )
        {
            DrawHelper::LinkedGEJunctionClosure2( eptLinesInfo[i].objId );
        }

        // 调整风流方向
        AutoDirection();
    }
}