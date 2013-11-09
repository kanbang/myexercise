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


// flag = 1 --> С��ͻ����
// flag = 2 --> ����ͻ����
// flag = 3 --> ����ͻ����
// flag = 4 --> �ش���ͻ����
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

// flag = 1 --> ú��Ԥ��ָ��
// flag = 2 --> ������Ԥ��ָ��1
// flag = 3 --> ������Ԥ��ָ��2
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

// flag = 1 --> ��������˹ӿ��
// flag = 2 --> �����������˹ӿ��
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
    //acutPrintf(_T("\n˫���Զ���..."));
    AcDbObjectIdArray objIds;
    ArxUtilHelper::GetPickSetEntity( objIds );
    if( objIds.length() != 1 ) return;

    // ��ʾ���ԶԻ���
    DisplayGasPropertyData( objIds[0] );
}

void DrawCmd::DrawMineBoundary( void )
{
    acutPrintf( _T( "\n���ƾ���߽����..." ) );

    AcGePoint3dArray pts;
    if( !GetPolygonByCmd( pts ) ) return;

    int len = pts.length();
    if( len < 2 )
    {
        acutPrintf( _T( "\n������Ҫ3������ܹ��ɾ���߽�" ) );
        return;
    }

    // ɾ�����еľ���߽�ͼԪ����ֻ֤��һ������߽�
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "MineBoundary" ), objIds );
    if( !objIds.isEmpty() ) ArxEntityHelper::EraseObjects2( objIds, true );

    MineBoundary* pBoundary = new MineBoundary();

    for( int i = 0; i < len; i++ ) pBoundary->addControlPoint( pts[i] );

    // ��ʼ�����ύ�����ݿ�
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
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ���ɿ���:" ) );
    if( !ArxUtilHelper::IsEqualType( _T( "Goaf" ), objId ) ) return;

    // �ɿ��������
    AcGePoint3dArray polygon;
    GetGoafPolygon( objId, polygon );
    if( polygon.length() < 3 ) return;


    AcGePoint3d pt;
    ArxUtilHelper::PromptPt( _T( "��ָ���ָ������: [����(D)]" ), pt );

    // ���Ҿ�������ı�
    int pos = GetClosePolygonEdge( polygon, pt );
    //acutPrintf(_T("\n�����λ��:%d"), pos);
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
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ���ɿ���:" ) );
    if( !ArxUtilHelper::IsEqualType( _T( "Goaf" ), objId ) ) return;

    // �ɿ��������
    AcGePoint3dArray polygon;
    GetGoafPolygon( objId, polygon );
    if( polygon.length() < 3 ) return;

    AcGePoint3d pt;
    ArxUtilHelper::PromptPt( _T( "��ָ��Ҫɾ���Ĳɿ����߽��: " ), pt );

    int pos = FindBoundaryPoint( polygon, pt );
    if( pos != -1 )
    {
        RemoveGoafBoundaryPoint( objId, pos );
    }
}