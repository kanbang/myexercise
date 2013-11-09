#include "StdAfx.h"

extern void FindAllGoafs( AcDbObjectIdArray& objIds );

/* 全局函数(BuildGoafRegion.cpp) */
extern void GetGoafPolygon( const AcDbObjectId& objId, AcGePoint3dArray& polygon );

/* 全局函数(DoubleLineHelper.cpp) */
extern void ExpandLinesOnPolygon( const AcDbVoidPtrArray& lines, const AcGePoint3dArray& polygon,
                                  AcGePoint3dArray& spts, AcGePoint3dArray& epts,
                                  AcGeDoubleArray& dirs, bool isInner );
extern void FilterLines( const AcDbVoidPtrArray& all_lines, AcDbVoidPtrArray& lines, bool findWS );

// 查找工作面与采空区间隔降压设置
void FindPressureFacility( const AcDbVoidPtrArray& lines, AcGePoint3dArray& spts, AcGePoint3dArray& epts, AcGeDoubleArray& dirs )
{
    // 查找工作面直线
    AcDbVoidPtrArray wsLines;
    FilterLines( lines, wsLines, true );

    // 查找所有的采空区
    AcDbObjectIdArray objIds;
    FindAllGoafs( objIds );

    for( int i = 0; i < objIds.length(); i++ )
    {
        // 获取采空区的多边形
        AcGePoint3dArray polygon;
        GetGoafPolygon( objIds[i], polygon );

        // 查找工作面的向内扩展边
        ExpandLinesOnPolygon( wsLines, polygon, spts, epts, dirs, true );
    }
}

void PrintPressFacility( const AcGePoint3dArray press_spts, const AcGePoint3dArray& press_epts, const AcGeDoubleArray& press_dirs )
{
    acutPrintf( _T( "打印工作面与采空区间隔降压设置: \n" ) );
    int n = press_spts.length();
    for( int i = 0; i < n; i++ )
    {
        AcGePoint3d spt = press_spts[i], ept = press_epts[i];
        double dir = press_dirs[i];
        acutPrintf( _T( "  (%.3f, %.3f) --> (%.3f, %.3f) 方向: %.3f\n" ), spt.x, spt.y, ept.x, ept.y, dir );
    }
    acutPrintf( _T( "\n" ) );
}