#include "stdafx.h"
#include "ContourCmd.h"

#include "GasDataLink.h"
#include "GasContourType.h"

#include "TriContour.h"
#include "ContourHelper.h"

#include "DrawContourDlg.h"
#include "ContourParamDlg.h"

#include "ConvexHullHelper.h"
#include "PolyLineJig.h"

#include "CADDrawHelper.h"

#include "../GasGE/PointGE.h"
#include "../ArxHelper/HelperClass.h"

/* ȫ�ֺ���(ʵ����ReadData.cpp) */
extern void ReadDataFromFile( const CString& fileName, AcGePoint3dArray& datas );

static void ReadDataByGasType( GasContourType gct, AcGePoint3dArray& datas )
{
    // �������е�GFP
    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( GetGasTypeName( gct ), objIds );
    if( objIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        PointGE* pGE = PointGE::cast( pObj ); // ���е���˹ͼԪ���Ǵ�PointGE����

        // ��ȡx,y����
        AcGePoint3d pt = pGE->getInsertPt();

        // ��ȡ����zֵ
        GasDataLink dl;
        dl.setDataSource( objIds[i] );
        dl.name = GetGasFieldName( gct );
        if( dl.updateData( false ) )
        {
            pt.z = dl.z;
        }

        datas.append( pt );
    }
    actrTransactionManager->endTransaction();
}

void ContourCmd::DrawGasContour()
{
    CAcModuleResourceOverride myResources;
    DrawContourDlg dlg;
    if( IDOK == dlg.DoModal() )
    {
        GasContourType gct = ( GasContourType )dlg.m_gct;
        double dz = dlg.m_dz;

        AcGePoint3dArray datas;
        ReadDataByGasType( gct, datas );
        if( datas.isEmpty() )
        {
            CString msg;
            msg.Format( _T( "û��%s���ã���ʹ�ò˵��򹤾�����������!!!" ), GetGasFieldName( gct ) );
            AfxMessageBox( msg );
            return;
        }

        CString layerName = GetGasContourLayerName( gct );

        // ɾ��ͼ���ϵ�����ͼ��ʵ��
        LayerHelper::DeleteAllEntitiesOnLayer( layerName );

        // �л�ͼ��
        LayerSwitch( layerName, true );

        // ���Ƶ�ֵ��
        ContourHelper::DrawCountour( datas, dz );
    }
}

static void SetContourParam( bool bSmooth, double textHeight )
{
    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( _T( "TriContour" ), objIds );
    if( objIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    for( int i = 0; i < objIds.length(); i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        TriContour* pContour = TriContour::cast( pObj );
        pContour->setTextHeight( textHeight );
        pContour->smooth( bSmooth );
    }
    actrTransactionManager->endTransaction();
}

void ContourCmd::ShowContourParamDlg()
{
    CAcModuleResourceOverride myResources;
    ContourParamDlg dlg;
    if( IDOK == dlg.DoModal() )
    {
        SetContourParam( dlg.m_bSmooth != 0, dlg.m_textHeight );
    }
}

void ContourCmd::RestoreLabel()
{
    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( _T( "TriContour" ), objIds );
    if( objIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    for( int i = 0; i < objIds.length(); i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        TriContour* pContour = TriContour::cast( pObj );
        pContour->label();
    }
    actrTransactionManager->endTransaction();
}

void ContourCmd::GridCaclTest()
{
    AcGePoint3dArray pts;

    PolyLineJig jig;
    if( !jig.doJig( pts ) ) return;

    AcGePoint3dArray H;
    //Graham_ConvexHull_3D(pts, H); // ʹ��Graphm�㷨
    Melkman_ConvexHull_3D( pts, H ); // ʹ��Melkman�㷨

    acutPrintf( _T( "\nʹ�ñպ϶���λ���͹��" ) );
    DrawPLines( H, true );

    // ʹ��AcDbExtent��ȡ��С�߽��
    AcDbExtents ext;
    int len = H.length();
    for( int i = 0; i < len; i++ ) ext.addPoint( H[i] );

    acutPrintf( _T( "\n��С��:(%.3f, %.3f,  %.3f)" ), ext.minPoint() );
    acutPrintf( _T( "\n����:(%.3f, %.3f,  %.3f)" ), ext.maxPoint() );

    AcGePoint3dArray datas;
    ReadDataFromFile( _T( "D:\\TestSite.txt" ), datas );
    double z = ContourHelper::GridCacl( datas, ext.minPoint(), ext.maxPoint(), 10, 4 );

    CString msg;
    msg.Format( _T( "����zֵ:%.5f" ), z );
    AfxMessageBox( msg );
    //acutPrintf(_T("\n����zֵ:%.5f"), z);
}

void ContourCmd::PointCaclTest()
{
    AcGePoint3d pt;
    if( !ArxUtilHelper::PromptPt( _T( "\n��ѡ��һ������" ), pt ) ) return;

    AcGePoint3dArray datas;
    ReadDataFromFile( _T( "D:\\TestSite.txt" ), datas );
    double z = ContourHelper::PointCacl( datas, pt, 4 );

    CString msg;
    msg.Format( _T( "����:(%.3f, %.3f)\nzֵ:%.3f" ), pt.x, pt.y, z );
    AfxMessageBox( msg );
}

void ContourCmd::PolyLineJigTest()
{
    AcGePoint3dArray pts;

    PolyLineJig jig;
    if( !jig.doJig( pts ) )
    {
        acutPrintf( _T( "\njig����ʧ��" ) );
    }
    else
    {
        acutPrintf( _T( "\njig���̳ɹ�" ) );
        acutPrintf( _T( "\nѰ��͹��..." ) );

        AcGePoint3dArray H;
        //Graham_ConvexHull_3D(pts, H); // ʹ��Graham�㷨
        Melkman_ConvexHull_3D( pts, H ); // ʹ��Melkman�㷨

        acutPrintf( _T( "\nʹ�ñպ϶���λ���͹��" ) );
        DrawPLines( H, true );

        acutPrintf( _T( "\nʹ����Բ����ԭʼ��" ) );
        int len = pts.length();
        for( int i = 0; i < len; i++ )
        {
            DrawEllipse( pts[i], 30, 30 );
        }
    }
}

void ContourCmd::twt()
{
    DrawMText( AcGePoint3d( 100, 200, 0 ), 0, _T( " �ҵ�̫�� " ), 50 );
    DrawMText( AcGePoint3d( 100, 300, 0 ), 0, _T( " �ҵ�̫�� " ), 20 );
    DrawMText( AcGePoint3d( 100, 400, 0 ), 0, _T( " 1234.56 " ), 50 );
    DrawMText( AcGePoint3d( 100, 500, 0 ), 0, _T( " 1234.56 " ), 10 );
    DrawMText( AcGePoint3d( 100, 600, 0 ), 0, _T( "  1234.56  " ), 10 );
    DrawMText( AcGePoint3d( 100, 700, 0 ), 0, _T( "1234.56" ), 10 );

    AcGePoint3d pt1( 0, 0, 0 ), pt2( 10, 0, 0 ), pt3( 20, 5, 0 );
    AcGeLineSeg3d line( pt1, pt2 );
    double dist = line.distanceTo( pt3 );

    acutPrintf( _T( "\n����: %.3f" ), dist );

    AcGePoint3d pt = line.closestPointTo( pt3 );
    acutPrintf( _T( "\n�����:(%.3f, %.3f, %.3f)" ), pt.x, pt.y, pt.z );
}

void ContourCmd::DrawBoundary()
{
    // δʵ��
    // ������Դ�ݲ���ȷ
}