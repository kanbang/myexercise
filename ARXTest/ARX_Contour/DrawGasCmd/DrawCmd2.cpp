#include "stdafx.h"
#include "DrawCmd.h"

#include "PolyLineJig.h"

#include "GOPDlg.h"
#include "GCPDlg.h"
#include "GPPDlg.h"
#include "PPPDlg.h"
#include "CSDCIDlg.h"
#include "WDCI1Dlg.h"
#include "WDCI2Dlg.h"
#include "WGFPDlg.h"
#include "TGFPDlg.h"
#include "CDHDlg.h"

#include "../GasGE/GOP.h"
#include "../GasGE/GCP.h"
#include "../GasGE/GPP.h"
#include "../GasGE/PPP.h"
#include "../GasGE/DCI.h"
#include "../GasGE/GFP.h"
#include "../GasGE/CDH.h"

#include "../GasGE/MineBoundary.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"


// flag = 1 --> 小型突出点
// flag = 2 --> 中型突出点
// flag = 3 --> 大型突出点
// flag = 4 --> 特大型突出点
void DrawCmd::DrawGOP2()
{
    CAcModuleResourceOverride myResources;

    GOPDlg dlg;
    dlg.DoModal();
}

void DrawCmd::DrawGCP2()
{
    CAcModuleResourceOverride myResources;

    GCPDlg dlg;
    dlg.DoModal();
}

void DrawCmd::DrawGPP2()
{
    CAcModuleResourceOverride myResources;

    GPPDlg dlg;
    dlg.DoModal();
}

void DrawCmd::DrawPPP2()
{
    CAcModuleResourceOverride myResources;

    PPPDlg dlg;
    dlg.DoModal();
}

// flag = 1 --> 煤层预测指标
// flag = 2 --> 工作面预测指标1
// flag = 3 --> 工作面预测指标2
void DrawCmd::DrawDCI2( int flag )
{
    CAcModuleResourceOverride myResources;
    if( flag == 1 )
    {
        CSDCIDlg dlg;
        dlg.DoModal();
    }
    else if( flag == 2 )
    {
        WDCI1Dlg dlg;
        dlg.DoModal();
    }
    else if( flag == 3 )
    {
        WDCI2Dlg dlg;
        dlg.DoModal();
    }
}

// flag = 1 --> 工作面瓦斯涌出
// flag = 2 --> 掘进工作面瓦斯涌出
void DrawCmd::DrawGFP2( int flag )
{
    CAcModuleResourceOverride myResources;

    if( flag == 1 )
    {
        WGFPDlg dlg;
        dlg.DoModal();
    }
    else if( flag == 2 )
    {
        TGFPDlg dlg;
        dlg.DoModal();
    }
}

void DrawCmd::DrawCDH2()
{
    CAcModuleResourceOverride myResources;
    CDHDlg dlg;
    dlg.DoModal();
}

static void DisplayGasPropertyData( const AcDbObjectId& objId )
{
    CAcModuleResourceOverride myResources;

    if( ArxUtilHelper::IsEqualType( _T( "GOP" ), objId ) )
    {
        GOPDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "GCP" ), objId ) )
    {
        GCPDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "GPP" ), objId ) )
    {
        GPPDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "PPP" ), objId ) )
    {
        PPPDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "CSDCI" ), objId ) )
    {
        CSDCIDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "WDCI1" ), objId ) )
    {
        WDCI1Dlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "WDCI2" ), objId ) )
    {
        WDCI2Dlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "WGFP" ), objId ) )
    {
        WGFPDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "TGFP" ), objId ) )
    {
        TGFPDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "CDH" ), objId ) )
    {
        CDHDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
}

void DrawCmd::DisplayDataByDoubleClick()
{
    //acutPrintf(_T("\n双击自定义..."));
    AcDbObjectIdArray objIds;
    ArxUtilHelper::GetPickSetEntity( objIds );
    if( objIds.length() != 1 ) return;

    // 显示属性对话框
    DisplayGasPropertyData( objIds[0] );
}

void DrawCmd::DrawMineBoundary( void )
{
    acutPrintf( _T( "\n绘制井田边界测试..." ) );

    AcGePoint3dArray pts;
    if( !GetPolygonByCmd( pts ) ) return;

    int len = pts.length();
    if( len < 2 )
    {
        acutPrintf( _T( "\n至少需要3个点才能构成井田边界" ) );
        return;
    }

    // 删除已有的井田边界图元，保证只有一个井田边界
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "MineBoundary" ), objIds );
    if( !objIds.isEmpty() ) ArxEntityHelper::EraseObjects2( objIds, true );

    MineBoundary* pBoundary = new MineBoundary();

    for( int i = 0; i < len; i++ ) pBoundary->addControlPoint( pts[i] );

    // 初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pBoundary ) ) delete pBoundary;
}

static void GetGoafPolygon( const AcDbObjectId& objId, AcGePoint3dArray& polygon )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk == pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        MineBoundary* pGoaf = MineBoundary::cast( pObj );
        if( pGoaf != 0 )
        {
            pGoaf->getControlPoints( polygon );
        }
    }

    actrTransactionManager->endTransaction();
}

static double DistOfPointToLine( const AcGePoint3d& spt, const AcGePoint3d& ept, const AcGePoint3d& pt )
{
    AcGeLine3d L( spt, ept );
    return L.distanceTo( pt );
}

static int GetClosePolygonEdge( const AcGePoint3dArray& polygon, const AcGePoint3d& pt )
{
    int pos = 0;
    double minDist = DistOfPointToLine( polygon[0], polygon[1], pt );
    int n = polygon.length();
    for( int i = 1; i < n; i++ )
    {
        double dist = DistOfPointToLine( polygon[i], polygon[( i + 1 ) % n], pt );
        if( minDist > dist )
        {
            minDist = dist;
            pos = i;
        }
    }
    return pos;
}

static void SplitGoafLine( const AcDbObjectId& objId, int pos, const AcGePoint3d& pt )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk == pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        MineBoundary* pGoaf = MineBoundary::cast( pObj );
        if( pGoaf != 0 )
        {
            pGoaf->splitLine( pos, pt );
        }
    }

    actrTransactionManager->endTransaction();
}

void DrawCmd::SplitGoaf()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个采空区:" ) );
    if( !ArxUtilHelper::IsEqualType( _T( "Goaf" ), objId ) ) return;

    // 采空区多边形
    AcGePoint3dArray polygon;
    GetGoafPolygon( objId, polygon );
    if( polygon.length() < 3 ) return;


    AcGePoint3d pt;
    ArxUtilHelper::PromptPt( _T( "请指定分割点坐标: [距离(D)]" ), pt );

    // 查找距离最近的边
    int pos = GetClosePolygonEdge( polygon, pt );
    //acutPrintf(_T("\n最近边位置:%d"), pos);
    SplitGoafLine( objId, pos, pt );
}

static int FindBoundaryPoint( const AcGePoint3dArray& polyon, const AcGePoint3d& pt )
{
    int pos = -1;

    int n = polyon.length();
    for( int i = 0; i < n; i++ )
    {
        if( pt == polyon[i] )
        {
            pos = i;
            break;
        }
    }
    return pos;
}

static void RemoveGoafBoundaryPoint( const AcDbObjectId& objId, int pos )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk == pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        MineBoundary* pGoaf = MineBoundary::cast( pObj );
        if( pGoaf != 0 )
        {
            pGoaf->removePoint( pos );
        }
    }

    actrTransactionManager->endTransaction();
}

void DrawCmd::RemoveGoafPoint()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个采空区:" ) );
    if( !ArxUtilHelper::IsEqualType( _T( "Goaf" ), objId ) ) return;

    // 采空区多边形
    AcGePoint3dArray polygon;
    GetGoafPolygon( objId, polygon );
    if( polygon.length() < 3 ) return;

    AcGePoint3d pt;
    ArxUtilHelper::PromptPt( _T( "请指定要删除的采空区边界点: " ), pt );

    int pos = FindBoundaryPoint( polygon, pt );
    if( pos != -1 )
    {
        RemoveGoafBoundaryPoint( objId, pos );
    }
}