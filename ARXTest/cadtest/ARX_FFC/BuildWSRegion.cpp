#include "stdafx.h"

#include "DoubleLine.h"
#include <algorithm>

/* 全局函数(RegionHelper.cpp) */
extern AcDbRegion* BuildPointPolygonRegion( const AcGePoint3dArray& polygon );
extern AcDbRegion* UniteRegion( AcDbVoidPtrArray& regions );

static AcDbRegion* BuildDoubleLineRegion( DoubleLine* pLine )
{
    // 获取多边形
    AcGePoint3dArray polygon;
    pLine->getPolygon( polygon );

    return BuildPointPolygonRegion( polygon );
}

AcDbRegion* BuildAndUniteRegion( const AcDbVoidPtrArray& lines )
{
    // 创建面域
    AcDbVoidPtrArray regions;
    int n = lines.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbRegion* pRegion = BuildDoubleLineRegion( ( DoubleLine* )lines[i] );
        if( pRegion != 0 )
        {
            regions.append( ( void* )pRegion );
        }
    }
    if( regions.isEmpty() )
    {
        return 0;
    }
    else
    {
        return UniteRegion( regions );
    }
}

// 获取最大的连通块编号
static int GetMaxCCNum( const AcDbIntArray& ccIds )
{
    int cc = -1;
    int n = ccIds.length();
    for( int i = 0; i < n; i++ )
    {
        if( ccIds[i] > cc )
        {
            cc = ccIds[i];
        }
    }
    return cc;
}

static void FindLinesByCCId( const AcDbIntArray& ccIds, const AcDbVoidPtrArray& lines, int c, AcDbVoidPtrArray& ccLines )
{
    int n = lines.length();
    for( int i = 0; i < n; i++ )
    {
        if( ccIds[i] == c )
        {
            ccLines.append( lines[i] );
        }
    }
}

// 可能有多个工作面面域
void BuildWSRegion( const AcDbIntArray& ccIds, const AcDbVoidPtrArray& lines, AcDbVoidPtrArray& regions )
{
    int cc = GetMaxCCNum( ccIds );
    for( int i = 0; i <= cc; i++ )
    {
        AcDbVoidPtrArray ccLines;
        FindLinesByCCId( ccIds, lines, i, ccLines );
        // 闭合之后,计算每条巷道(或工作面)的多边形面域
        // 最终合并成一个或多个面域
        AcDbRegion* pRegion = BuildAndUniteRegion( ccLines );
        if( pRegion != 0 )
        {
            regions.append( pRegion );
        }
    }
}
