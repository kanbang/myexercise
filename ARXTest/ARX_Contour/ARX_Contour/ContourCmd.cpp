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

/* 全局函数(实现在ContourTest.cpp) */
// 临时使用
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
        acutPrintf( _T( "\njig过程失败" ) );
        return;
    }
    if( pts.length() < 3 )
    {
        acutPrintf( _T( "多边形的点个数不能少于3个" ) );
        return;
    }

    acutPrintf( _T( "\n寻找凸包..." ) );

    AcGePoint3dArray H;
    //Graham_ConvexHull_3D(pts, H); // 使用Graham算法
    if( !Melkman_ConvexHull_3D( pts, H ) ) // 使用Melkman算法
    {
        acutPrintf( _T( "\n寻找凸包失败" ) );
        return;
    }

    acutPrintf( _T( "\n使用闭合多边形绘制凸包" ) );
    DrawPLines( H, true );

    acutPrintf( _T( "\n使用椭圆绘制原始点" ) );
    int len = pts.length();
    for( int i = 0; i < len; i++ )
    {
        DrawEllipse( pts[i], 30, 30 );
    }
}
