#include "stdafx.h"
#include "ContourCmd.h"

#include "ARX_GeoAlgo.h"
#include "PolyLineJig.h"
#include "ContourHelper.h"
#include "CADDrawHelper.h"

#include "DrawContourDlg.h"
#include "ContourParamDlg.h"
#include "ContourLayerDlg.h"
#include "ContourColorDlg.h"
#include "PointForecastDlg.h"
#include "RegionForecastDlg.h"

#include "../ArxHelper/HelperClass.h"
#include "../ContourGE/ContourLine.h"
#include "../ContourGE/ContourFill.h"

/* ȫ�ֺ���(ʵ����ContourTest.cpp) */
// ��ʱʹ��
extern void ReadDataFromFile( const CString& fileName, AcGePoint3dArray& datas );

void ContourCmd::ShowContourDlg()
{
    DrawContourDlg dlg;
    dlg.DoModal();
}

void ContourCmd::ShowContourParamDlg()
{
    CAcModuleResourceOverride myResources;
    ContourParamDlg dlg;
    dlg.DoModal();
}

void ContourCmd::ShowContourLayerDlg()
{
    CAcModuleResourceOverride myResources;
    ContourLayerDlg dlg;
    dlg.DoModal();
}

void ContourCmd::ShowContourColorDlg()
{
    CAcModuleResourceOverride myResources;

    ContourColorDlg dlg;
    dlg.DoModal();
}

void ContourCmd::ShowRegionForecastDlg()
{
    CAcModuleResourceOverride myResources;
    RegionForecastDlg dlg;
    dlg.DoModal();
}

void ContourCmd::ShowPointForecastDlg()
{
    CAcModuleResourceOverride myResources;
    PointForecastDlg dlg;
    dlg.DoModal();

    //extern void PointCaclTest();
    //PointCaclTest();
}

void ContourCmd::PolyLineJigTest()
{
    AcGePoint3dArray pts;

    //if(!GetPolygonByJig(pts))
    if( !GetPolygonByCmd( pts ) )
    {
        acutPrintf( _T( "\njig����ʧ��" ) );
        return;
    }
    if( pts.length() < 3 )
    {
        acutPrintf( _T( "����εĵ������������3��" ) );
        return;
    }

    acutPrintf( _T( "\nѰ��͹��..." ) );

    AcGePoint3dArray H;
    //Graham_ConvexHull_3D(pts, H); // ʹ��Graham�㷨
    if( !Melkman_ConvexHull_3D( pts, H ) ) // ʹ��Melkman�㷨
    {
        acutPrintf( _T( "\nѰ��͹��ʧ��" ) );
        return;
    }

    acutPrintf( _T( "\nʹ�ñպ϶���λ���͹��" ) );
    DrawPLines( H, true );

    acutPrintf( _T( "\nʹ����Բ����ԭʼ��" ) );
    int len = pts.length();
    for( int i = 0; i < len; i++ )
    {
        DrawEllipse( pts[i], 30, 30 );
    }
}
