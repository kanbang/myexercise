#include "StdAfx.h"

/* 全局函数(RegionHelper.cpp) */
extern AcDbRegion* BuildPointPolygonRegion( const AcGePoint3dArray& polygon );

/* 全局函数(DoubleLineHelper.cpp) */
extern void AdjustPointPolygon( const AcDbVoidPtrArray& lines, const AcGePoint3dArray& polygon, AcGePoint3dArray& ext_polygon, bool isInner );

/* 全局函数(Tool.cpp) */
extern void FindAllGoafs( AcDbObjectIdArray& objIds );
extern void GetGoafPolygon( const AcDbObjectId& objId, AcGePoint3dArray& polygon );

// 创建采空区面域(可能有多个采空区)
void BuildGoafRegion( const AcDbVoidPtrArray& lines, AcDbVoidPtrArray& regions )
{
    // 查找所有的采空区
    AcDbObjectIdArray objIds;
    FindAllGoafs( objIds );

    for( int i = 0; i < objIds.length(); i++ )
    {
        // 获取采空区的多边形
        AcGePoint3dArray polygon;
        GetGoafPolygon( objIds[i], polygon );

        // 调整采空区多边形(向内扩展)
        AcGePoint3dArray ext_polygon;
        AdjustPointPolygon( lines, polygon, ext_polygon, true );

        // 构建面域
        AcDbRegion* pRegion = BuildPointPolygonRegion( ext_polygon );
        if( pRegion != 0 )
        {
            regions.append( pRegion );
        }
    }
}