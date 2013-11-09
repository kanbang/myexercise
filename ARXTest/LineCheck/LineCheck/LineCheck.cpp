#include "StdAfx.h"

#include "CADDrawHelper.h"
#include "LayerHelper.h"
#include "ConvexHullHelper.h"

// 设置线宽
static void SeLineWidth_Helper( AcDbEntity* pEnt )
{
    if( pEnt->isKindOf( AcDbPolyline::desc() ) )
    {
        AcDbPolyline* pPline = AcDbPolyline::cast( pEnt );
        /*
        int count = pPline->numVerts();
        for(int i=0;i<count;i++)
        {
        	pPline->setWidthsAt(i, width, width);
        }
        */
        pPline->setConstantWidth( 0 );
    }
    pEnt->setLineWeight( AcDb::kLnWtByLwDefault ); // 默认
    //pEnt->setLineWeight(kLnWtByLayer); // 随层
    // 貌似使用0无效
}

static void PrintPt( long id, bool se, const AcGePoint3d& pt )
{
    /*if(IsInRange(pt))
    {*/
    acutPrintf( _T( "\nid=%ld, %s, %.15f, %.15f" ), id, ( se ? _T( "始点" ) : _T( "末点" ) ), pt.x, pt.y );
    //}
}

static void GetPts_Helper( AcDbEntity* pEnt, AcGePoint3dArray& pts )
{
    /*if(pEnt->isKindOf(AcDbPolyline::desc()))
    {
    	AcDbPolyline* pPline = AcDbPolyline::cast(pEnt);
    	int count = pPline->numVerts();
    	for(int i=0;i<count;i++)
    	{
    		AcGePoint3d pt;
    		pPline->getPointAt(i, pt);
    		if(!pts.contains(pt)) pts.append(pt);
    	}
    }
    else */if( pEnt->isKindOf( AcDbLine::desc() ) )
    {
        AcDbLine* pLine = AcDbLine::cast( pEnt );
        AcGePoint3d spt = pLine->startPoint();
        AcGePoint3d ept = pLine->endPoint();
        /*pLine->getStartPoint(spt);
        pLine->getEndPoint(ept);*/
        if( spt == ept ) return;

        long id = pLine->objectId().asOldId();
        /*double L = (spt - ept).length();

        if(L < 1)
        {
        	PrintPt(id, true, spt);
        	PrintPt(id, false, ept);
        	acutPrintf(_T("\n长度:%.15f"), L);
        }*/

        //PrintPt(id, true, spt);
        //PrintPt(id, false, ept);

        if( !pts.contains( spt ) )
        {
            //if(L<1) acutPrintf(_T("\n分支%d始点添加到集合"), id);
            pts.append( spt );
        }
        else
        {
            //if(L<1) acutPrintf(_T("\nid=%ld, 的始点%.15f, %.15f已在集合中"), id, spt.x, spt.y);
        }
        if( !pts.contains( ept ) )
        {
            //if(L<1) acutPrintf(_T("\nid=%d末点添加到集合"), id);
            pts.append( ept );
        }
        else
        {
            //if(L<1) acutPrintf(_T("\nid=%ld的末点%.15f, %.15f已在集合中"), id, ept.x, ept.y);
        }
    }
}

static void GetAllPtsOnLayer( AcGePoint3dArray& pts, const CString& layerName )
{
    AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();

    AcDbBlockTable* pBlkTbl;
    pDB->getSymbolTable( pBlkTbl, AcDb::kForRead );

    AcDbBlockTableRecord* pBlkTblRcd;
    pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();

    AcDbBlockTableRecordIterator* pBlkTblRcdItr;
    pBlkTblRcd->newIterator( pBlkTblRcdItr );
    for ( pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step() )
    {
        AcDbEntity* pEnt;
        if( Acad::eOk != pBlkTblRcdItr->getEntity( pEnt, AcDb::kForRead ) ) continue;

        if( layerName.CompareNoCase( pEnt->layer() ) == 0 ) // 只选择指定图层上的所有直线
        {
            // 获取点坐标
            GetPts_Helper( pEnt, pts );
        }
        pEnt->close();
    }
    delete pBlkTblRcdItr;

    pBlkTblRcd->close();

}

static void DeleteAllEntsOnLayer( const CString& layerName )
{
    AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
    AcDbBlockTable* pBlkTbl;
    pDB->getSymbolTable( pBlkTbl, AcDb::kForRead );

    AcDbBlockTableRecord* pBlkTblRcd;
    pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();

    AcDbBlockTableRecordIterator* pBlkTblRcdItr;
    pBlkTblRcd->newIterator( pBlkTblRcdItr );
    for ( pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step() )
    {
        AcDbEntity* pEnt;
        if( Acad::eOk != pBlkTblRcdItr->getEntity( pEnt, AcDb::kForWrite ) ) continue;

        if( layerName.CompareNoCase( pEnt->layer() ) == 0 )
        {
            pEnt->erase( true );
        }
        pEnt->close();
    }
    delete pBlkTblRcdItr;

    pBlkTblRcd->close();
}

static void ZeroLineWidth_Helper( const CString& layerName )
{
    AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
    AcDbBlockTable* pBlkTbl;
    pDB->getSymbolTable( pBlkTbl, AcDb::kForRead );

    AcDbBlockTableRecord* pBlkTblRcd;
    pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();

    AcDbBlockTableRecordIterator* pBlkTblRcdItr;
    pBlkTblRcd->newIterator( pBlkTblRcdItr );
    for ( pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step() )
    {
        AcDbEntity* pEnt;
        if( Acad::eOk != pBlkTblRcdItr->getEntity( pEnt, AcDb::kForWrite ) ) continue;

        if( layerName.CompareNoCase( pEnt->layer() ) == 0 ) // 只选择指定图层
        {
            //acutPrintf(_T("\n第%d图元所在图层:%s"), i++, pEnt->layer());
            // 设置宽度
            SeLineWidth_Helper( pEnt );
        }
        pEnt->close();
    }
    delete pBlkTblRcdItr;

    pBlkTblRcd->close();
}

static void ExecuteZoomCommand2( const AcGePoint3d& minPt, const AcGePoint3d& maxPt )
{
    /*
     * 发送命令方法(二)
     * 注：acedCommand方法在modeless dialog中无法使用
     * 参考: "Code Differences under the Application Execution Context" topic in ARX docs
     */
    // 发送cad命令：ZOOM-->W (缩放到图元的窗口范围)
    acedCommand( RTSTR, _T( "ZOOM" ), RTSTR, _T( "W" ), RT3DPOINT, asDblArray( minPt ), RT3DPOINT, asDblArray( maxPt ), 0 );
    //acedCommand(RTSTR, _T("ZOOM"), RTSTR, _T("A"), 0);
}

static void ZoomWindow( const AcGePoint3d& minPt, const AcGePoint3d& maxPt, double radius )
{
    AcGeVector3d v = maxPt - minPt;
    v.normalize();
    double c = 1 + 0.618;

    ExecuteZoomCommand2( minPt - v * radius * c, maxPt + v * radius * c );
}

static void FixLinePoint( AcDbEntity* pEnt, const AcGePoint3d& pt1, const AcGePoint3d& pt2 )
{
    if( pEnt->isKindOf( AcDbLine::desc() ) )
    {
        AcDbLine* pLine = AcDbLine::cast( pEnt );
        AcGePoint3d spt, ept;
        pLine->getStartPoint( spt );
        pLine->getEndPoint( ept );

        if( ( pt1 != spt ) && ( pt2 == ept ) )
        {
            //acutPrintf(_T("\n找到直线，直线的末点等于p1"));
            pLine->setEndPoint( pt1 ); // 全部处理到p1上
        }
        else if( ( pt2 == spt ) && ( pt1 != ept ) )
        {
            //acutPrintf(_T("\n找到直线，直线的始点等于p1"));
            pLine->setStartPoint( pt1 );
        }
    }
}

static void FixLinePoints_Helper( const CString& layerName, const AcGePoint3d& pt1, const AcGePoint3d& pt2 )
{
    AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
    AcDbBlockTable* pBlkTbl;
    pDB->getSymbolTable( pBlkTbl, AcDb::kForRead );

    AcDbBlockTableRecord* pBlkTblRcd;
    pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();

    AcDbBlockTableRecordIterator* pBlkTblRcdItr;
    pBlkTblRcd->newIterator( pBlkTblRcdItr );
    for ( pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step() )
    {
        AcDbEntity* pEnt;
        if( Acad::eOk != pBlkTblRcdItr->getEntity( pEnt, AcDb::kForWrite ) ) continue;

        if( layerName.CompareNoCase( pEnt->layer() ) == 0 ) // 只选择指定图层
        {
            FixLinePoint( pEnt, pt1, pt2 );
        }
        pEnt->close();
    }
    delete pBlkTblRcdItr;

    pBlkTblRcd->close();
}

struct SEPoint
{
    bool operator==( const SEPoint& sept )
    {
        return ( ( sept.spt == spt && sept.ept == ept ) ||
                 ( sept.ept == spt && sept.spt == ept ) );
    }
    AcGePoint3d spt;
    AcGePoint3d ept;
};

static bool GetSEPt( AcDbEntity* pEnt, AcGePoint3d& spt, AcGePoint3d& ept )
{
    if( !pEnt->isKindOf( AcDbLine::desc() ) ) return false;
    AcDbLine* pLine = AcDbLine::cast( pEnt );
    pLine->getStartPoint( spt );
    pLine->getEndPoint( ept );
    return true;
}

// 查找重合直线
static bool FindSuperpositionLine_Helper( const CString& layerName, AcGePoint3d& spt, AcGePoint3d& ept )
{
    AcArray<SEPoint> ses;

    AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
    AcDbBlockTable* pBlkTbl;
    pDB->getSymbolTable( pBlkTbl, AcDb::kForRead );

    AcDbBlockTableRecord* pBlkTblRcd;
    pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();

    bool ret = false;
    AcDbBlockTableRecordIterator* pBlkTblRcdItr;
    pBlkTblRcd->newIterator( pBlkTblRcdItr );
    for ( pBlkTblRcdItr->start(); !pBlkTblRcdItr->done() && !ret; pBlkTblRcdItr->step() )
    {
        AcDbEntity* pEnt;
        if( Acad::eOk != pBlkTblRcdItr->getEntity( pEnt, AcDb::kForWrite ) ) continue;

        if( layerName.CompareNoCase( pEnt->layer() ) == 0 ) // 只选择指定图层
        {
            if( GetSEPt( pEnt, spt, ept ) )
            {
                SEPoint se;
                se.spt = spt;
                se.ept = ept;
                if( ses.contains( se ) )
                {
                    ret = true;
                }
                else
                {
                    ses.append( se );
                }
            }
        }
        pEnt->close();
    }
    delete pBlkTblRcdItr;

    pBlkTblRcd->close();

    return ret;
}

void ZeroLineWidth()
{
    CString srcLayer = _T( "巷道" ); // 源图层
    CString msg;
    msg.Format( _T( "\n清除【%s】图层上的所有多段线和直线的线宽..." ), srcLayer );
//	acutPrintf(_T("\n清除【%s】图层上的所有多段线和直线的线宽..."), srcLayer);
    AfxMessageBox( msg );
    ZeroLineWidth_Helper( srcLayer );
}

void DrawLinePoints()
{
    // 获取当前图层名称
    CString curLayer = LayerHelper::GetCurrentLayerName();
    acutPrintf( _T( "\n当前图层:【%s】" ), curLayer );

    CString srcLayer = _T( "巷道" ); // 源图层
    acutPrintf( _T( "\n获取【%s】图层上的所有直线的点坐标" ), srcLayer );
    AcGePoint3dArray pts;
    GetAllPtsOnLayer( pts, srcLayer );
    if( pts.isEmpty() ) return;

    CString targetLayer = _T( "临时" );
    acutPrintf( _T( "\n新建【%s】图层；如果图层存在，则删除图层上的所有图形..." ), targetLayer );
    LayerHelper::AddLayer( targetLayer ); // 如果存在则不建立
    DeleteAllEntsOnLayer( targetLayer );

    // 将新建图层设置为当前图层
    LayerHelper::SetCurrentLayer( targetLayer );

    double radius = 10;
    int len = pts.length();
    for( int i = 0; i < len; i++ )
    {
        DrawEllipse( pts[i], radius, radius );
    }
    LayerHelper::SetCurrentLayer( curLayer );

    AcGePoint3d pt1, pt2;
    ClosestPoint_3D( pts, pt1, pt2 );

    acutPrintf( _T( "\n最近点对坐标:(%.15f, %.15f)-->(%.15f, %.15f)" ), pt1.x, pt1.y, pt2.x, pt2.y );
    acutPrintf( _T( "\n最近点对距离:%.15f\n" ), ( pt2 - pt1 ).length() );

    ZoomWindow( pt1, pt2, radius );
}

void FixLinePoints()
{
    CString srcLayer = _T( "巷道" ); // 源图层
    acutPrintf( _T( "\n获取【%s】图层上的所有直线的点坐标" ), srcLayer );
    AcGePoint3dArray pts;
    GetAllPtsOnLayer( pts, srcLayer );
    if( pts.isEmpty() ) return;

    AcGePoint3d pt1, pt2;
    ClosestPoint_3D( pts, pt1, pt2 );

    ZoomWindow( pt1, pt2, 10 );

    AcGeVector3d v = pt2 - pt1;
    CString msg;
    msg.Format( _T( "最近点对距离:%.15f\n是否修正???" ), v.length() );
    if( IDYES == AfxMessageBox( msg, MB_YESNO ) )
    {
        FixLinePoints_Helper( srcLayer, pt1, pt2 );
    }
}

void FindSuperpositionLine()
{
    CString srcLayer = _T( "巷道" ); // 源图层
    acutPrintf( _T( "\n获取【%s】图层上的所有直线的点坐标" ), srcLayer );

    AcGePoint3d spt, ept;
    if( FindSuperpositionLine_Helper( srcLayer, spt, ept ) )
    {
        acutPrintf( _T( "\n重合直线端点坐标:(%.15f, %.15f)-->(%.15f, %.15f)\n" ), spt.x, spt.y, ept.x, ept.y );
        ZoomWindow( spt, ept, 10 );
    }
    else
    {
        CString msg;
        msg.Format( _T( "【%s】图层上没有重合的直线" ), srcLayer );
        AfxMessageBox( msg );
    }
}
