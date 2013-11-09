#include "StdAfx.h"

extern void FindAllGoafs( AcDbObjectIdArray& objIds );

/* 全局函数(BuildGoafRegion.cpp) */
extern void GetGoafPolygon( const AcDbObjectId& objId, AcGePoint3dArray& polygon );

/* 全局函数(DoubleLineHelper.cpp) */
extern void FilterLines( const AcDbVoidPtrArray& all_lines, AcDbVoidPtrArray& lines, bool findWS );
extern int FindLineByPoints( const AcDbVoidPtrArray& lines, const AcGePoint3d& spt, const AcGePoint3d& ept );
extern void AdjustLinePolygon( const AcDbVoidPtrArray& lines, const AcGePoint3dArray& polygon, AcGePoint3dArray& line_polygon, bool isInner );
extern void AdjustPointPolygon( const AcDbVoidPtrArray& lines, const AcGePoint3dArray& polygon, AcGePoint3dArray& ext_polygon, bool isInner );

/* 全局函数(GeoTool.cpp) */
extern int ClockWise( const AcGePoint3dArray& polygon );
extern void PartitionPolygon( const AcGePoint3dArray& polygon, int k, AcDbIntArray& opposite, double c );
extern void CaclDirections( bool isClockWise, const AcGePoint3dArray& spts, const AcGePoint3dArray& epts, AcGeDoubleArray& dirs );
extern void FindPolygonColinearEdges( const AcGePoint3dArray& polygons, const AcDbIntArray& polygon_counts, AcDbIntArray& colinearEdges );

void BuildGoafPolygonArray( const AcDbObjectIdArray& objIds, AcGePoint3dArray& polygons, AcDbIntArray& polygon_counts )
{
    for( int i = 0; i < objIds.length(); i++ )
    {
        // 获取采空区的多边形
        AcGePoint3dArray polygon;
        GetGoafPolygon( objIds[i], polygon );

        polygons.append( polygon );
        polygon_counts.append( polygon.length() );
    }
}

static bool PartitionGoafPolygon( const AcDbVoidPtrArray& ws_lines,
                                  const AcGePoint3dArray& polygon,
                                  AcDbIntArray& parTypes )
{
    AcDbIntArray ws_lines_pos;

    int n = polygon.length();
    for( int i = 0; i < n; i++ )
    {
        parTypes.append( 0 ); // 初始化为0(表示错误状态)
        if( FindLineByPoints( ws_lines, polygon[i], polygon[( i + 1 ) % n] ) != -1 )
        {
            ws_lines_pos.append( i );
        }
    }

    acutPrintf( _T( "\n采空区上工作面个数:%d" ), ws_lines_pos.length() );
    if( ws_lines_pos.isEmpty() ) return false;
    // 假设采空区的工作面不能超过2个
    if( ws_lines_pos.length() > 2 ) return false;
    // 2个工作面的位置：
    //		1) 工作面相邻
    //		2) 工作面互为对面
    //		3) 其它情况

    // 初始化划分类型parTypes;
    //	1 -- 两帮
    //	2 -- 工作面
    //	3 -- 开切眼
    for( int i = 0; i < n; i++ )
    {
        parTypes[i] = 1; // 默认为两帮
    }
    for( int i = 0; i < ws_lines_pos.length(); i++ )
    {
        parTypes[ws_lines_pos[i]] = 2;
    }

    // 这个数只是一个近似估计值
    // 黄金比例0.382/0.618，而其衍生比例0.236/0.764，0.146/0.854，0.5/0.5
    // http://blog.eastmoney.com/junjun1997/blog_130654962.html
    const double c = 0.854;

    // 查找公共的开切眼
    // 使用数组cut_counts标记分支被视为开切眼的次数
    AcDbIntArray cut_counts;
    for( int i = 0; i < n; i++ )
    {
        cut_counts.append( 0 );
    }
    for( int i = 0; i < ws_lines_pos.length(); i++ )
    {
        AcDbIntArray cuts;
        PartitionPolygon( polygon, ws_lines_pos[i], cuts, c );
        for( int j = 0; j < cuts.length(); j++ )
        {
            cut_counts[cuts[j]]++;
        }
    }
    for( int i = 0; i < cut_counts.length(); i++ )
    {
        // 如果被视为开切眼的次数等于工作面的个数
        // 则该分支视为公共的开切眼
        if( cut_counts[i] == ws_lines_pos.length() )
        {
            parTypes[i] = 3;
        }
    }

    return true;
}

static bool PartitionGoafPolygon( const AcDbVoidPtrArray& ws_lines,
                                  const AcGePoint3dArray& polygons,
                                  const AcDbIntArray& polygon_counts,
                                  int k,
                                  AcDbIntArray& parTypes )
{
    int s = 0;
    for( int i = 0; i < k; i++ )
    {
        s += polygon_counts[i];
    }
    int t = s + polygon_counts[k];

    AcGePoint3dArray goaf_polygon;
    for( int i = s; i < t; i++ )
    {
        goaf_polygon.append( polygons[i] );
    }

    return PartitionGoafPolygon( ws_lines, goaf_polygon, parTypes );
}

static void PartitionGoafPolygons( const AcDbVoidPtrArray& ws_lines,
                                   AcGePoint3dArray& polygons,
                                   AcDbIntArray& polygon_counts,
                                   AcDbIntArray& parTypes )
{
    for( int i = 0; i < polygon_counts.length(); i++ )
    {
        AcDbIntArray goaf_parTypes;
        PartitionGoafPolygon( ws_lines, polygons, polygon_counts, i, goaf_parTypes );

        parTypes.append( goaf_parTypes );
    }
}

static void ExplodeGoafPolygon( const AcGePoint3dArray& polygon,
                                const AcGePoint3dArray& inner_polygon,
                                const AcGePoint3dArray& outer_polygon,
                                AcGePoint3dArray& spts, AcGePoint3dArray& epts )
{
    int n = polygon.length();
    for( int i = 0; i < n; i++ )
    {
        AcGePoint3d spt = polygon[i], ept = polygon[( i + 1 ) % n];

        // 如果向外扩展的坐标没有发生变化
        // 则使向内扩展坐标
        if( spt == outer_polygon[2 * i] || ept == outer_polygon[2 * i + 1] )
        {
            spt = inner_polygon[i];
            ept = inner_polygon[( i + 1 ) % n];
        }
        else
        {
            spt = outer_polygon[2 * i];
            ept = outer_polygon[2 * i + 1];
        }

        spts.append( spt );
        epts.append( ept );
    }
}

static void AdjustAndExplodeGoafPolygon( const AcDbVoidPtrArray& lines,
        const AcGePoint3dArray& polygons,
        const AcDbIntArray& polygon_counts,
        int k,
        AcGePoint3dArray& spts,
        AcGePoint3dArray& epts,
        AcGeDoubleArray& dirs )
{
    int s = 0;
    for( int i = 0; i < k; i++ )
    {
        s += polygon_counts[i];
    }
    int t = s + polygon_counts[k];

    AcGePoint3dArray goaf_polygon;
    for( int i = s; i < t; i++ )
    {
        goaf_polygon.append( polygons[i] );
    }

    // 调整采空区多边形(向内)
    // 保证点是彼此相连的
    AcGePoint3dArray inner_polygon;
    AdjustPointPolygon( lines, goaf_polygon, inner_polygon, true );

    // 调整采空区多边形(向外)
    // 不保证点是彼此相连的
    AcGePoint3dArray outer_polygon;
    AdjustLinePolygon( lines, goaf_polygon, outer_polygon, false );

    // 分解多边形为直线数组
    ExplodeGoafPolygon( goaf_polygon, inner_polygon, outer_polygon, spts, epts );
    //ExplodeLinePolygon(outer_polygon, spts, epts);

    // 计算方向(垂直分支，向采空区内)
    CaclDirections( ClockWise( goaf_polygon ) == -1, spts, epts, dirs );
}

static void AdjustAndExplodeGoafPolygons( const AcDbVoidPtrArray& lines,
        const AcGePoint3dArray& polygons,
        const AcDbIntArray& polygon_counts,
        AcGePoint3dArray& spts,
        AcGePoint3dArray& epts,
        AcGeDoubleArray& dirs )
{
    for( int i = 0; i < polygon_counts.length(); i++ )
    {
        AcGePoint3dArray goaf_spts, goaf_epts;
        AcGeDoubleArray goaf_dirs;
        AdjustAndExplodeGoafPolygon( lines, polygons, polygon_counts, i, goaf_spts, goaf_epts, goaf_dirs );

        spts.append( goaf_spts );
        epts.append( goaf_epts );
        dirs.append( goaf_dirs );
    }
}

static void AppendNewGoafPolygon( const AcGePoint3dArray& polygons,
                                  const AcDbIntArray& polygon_counts,
                                  const AcDbIntArray& colinearEdges,
                                  const AcDbIntArray& parTypes,
                                  const AcGePoint3dArray& ex_spts,
                                  const AcGePoint3dArray& ex_epts,
                                  const AcGeDoubleArray& ex_dirs,
                                  const AcDbIntArray& linePos,
                                  int k,
                                  AcGePoint3dArray& spts,
                                  AcGePoint3dArray& epts,
                                  AcGeDoubleArray& dirs,
                                  AcDbIntArray& gas_types,
                                  AcDbIntArray& gas_linePos )
{
    int s = 0;
    for( int i = 0; i < k; i++ )
    {
        s += polygon_counts[i];
    }
    int t = s + polygon_counts[k];

    for( int i = s; i < t; i++ )
    {
        if( colinearEdges[i] == 0 )
        {
            spts.append( ex_spts[i] );
            epts.append( ex_epts[i] );
            dirs.append( ex_dirs );
            gas_types.append( parTypes[i] );
            gas_linePos.append( linePos[i] );
        }
    }
}

static void FindPolygonLinePos( const AcDbVoidPtrArray& lines,
                                const AcGePoint3dArray& polygons,
                                const AcDbIntArray& polygon_counts,
                                int k,
                                AcDbIntArray& linePos )
{
    int s = 0;
    for( int i = 0; i < k; i++ )
    {
        s += polygon_counts[i];
    }
    int t = s + polygon_counts[k];

    AcGePoint3dArray polygon;
    for( int i = s; i < t; i++ )
    {
        polygon.append( polygons[i] );
    }

    int n = polygon.length();
    for( int i = 0; i < n; i++ )
    {
        linePos.append( FindLineByPoints( lines, polygon[i], polygon[( i + 1 ) % n] ) );
    }
}

static void FindGoafPolygonLinePos( const AcDbVoidPtrArray& lines,
                                    const AcGePoint3dArray& polygons,
                                    const AcDbIntArray& polygon_counts,
                                    AcDbIntArray& linePos )
{
    int n = polygon_counts.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbIntArray ex_linePos;
        FindPolygonLinePos( lines, polygons, polygon_counts, i, ex_linePos );
        linePos.append( ex_linePos );
    }
}

static void AdjustGoafPolygon( const AcDbVoidPtrArray& lines,
                               const AcGePoint3dArray& polygons,
                               const AcDbIntArray& polygon_counts,
                               const AcDbIntArray& colinearEdges,
                               const AcDbIntArray& parTypes,
                               AcGePoint3dArray& spts,
                               AcGePoint3dArray& epts,
                               AcGeDoubleArray& dirs,
                               AcDbIntArray& gas_types,
                               AcDbIntArray& gas_linePos )
{
    // 查找所有采空区边对应的直线
    AcDbIntArray goaf_linePos;
    FindGoafPolygonLinePos( lines, polygons, polygon_counts, goaf_linePos );

    // 分解所有的分支
    AcGePoint3dArray ex_spts, ex_epts;
    AcGeDoubleArray ex_dirs;
    AdjustAndExplodeGoafPolygons( lines, polygons, polygon_counts, ex_spts, ex_epts, ex_dirs );

    assert( ex_spts.length() == polygons.length() );

    for( int i = 0; i < polygon_counts.length(); i++ )
    {
        AppendNewGoafPolygon(
            polygons, polygon_counts,
            colinearEdges, parTypes,
            ex_spts, ex_epts, ex_dirs,
            goaf_linePos,
            i,
            spts, epts, dirs,
            gas_types,
            gas_linePos );
    }
}

// 查找采空区的其它边
void FindGasBoundary( const AcDbObjectIdArray& objIds,
                      const AcDbVoidPtrArray& lines,
                      AcGePoint3dArray& spts,
                      AcGePoint3dArray& epts,
                      AcGeDoubleArray& dirs,
                      AcDbIntArray& gas_types,
                      AcDbObjectIdArray& gas_objIds )
{
    // 查找所有的采空区
    AcDbObjectIdArray goaf_objIds;
    FindAllGoafs( goaf_objIds );

    // 将采空区多边形转换成一个1维数组
    AcGePoint3dArray polygons;
    AcDbIntArray polygon_counts;
    BuildGoafPolygonArray( goaf_objIds, polygons, polygon_counts );

    // 标记采空区分支是否与其它采空区有共线边
    AcDbIntArray colinearEdges;
    FindPolygonColinearEdges( polygons, polygon_counts, colinearEdges );

    // 查找所有的工作面
    AcDbVoidPtrArray ws_lines;
    FilterLines( lines, ws_lines, true );

    // 划分采空区多边形(工作面、两帮、开切眼)
    AcDbIntArray parTypes;
    PartitionGoafPolygons( ws_lines, polygons, polygon_counts, parTypes );

    assert( parTypes.length() == polygons.length() );

    // 工作面需要特殊处理
    AcDbIntArray gas_linePos;
    AdjustGoafPolygon(
        lines, polygons, polygon_counts,
        colinearEdges, parTypes,
        spts, epts, dirs,
        gas_types, gas_linePos );

    assert( gas_types.length() == gas_linePos.length() );

    for( int i = 0; i < gas_linePos.length(); i++ )
    {
        int pos = gas_linePos[i];
        if( pos != -1 )
        {
            gas_objIds.append( objIds[pos] );
        }
        else
        {
            gas_objIds.append( AcDbObjectId::kNull );
        }
    }
}

// 至少应有1个工作面瓦斯边界
// gas_type =
bool HasWSGasBoundary( const AcDbIntArray& gas_types )
{
    return gas_types.contains( 2 );
}

// 初始化划分类型parTypes;
//	1 -- 两帮
//	2 -- 工作面
//	3 -- 开切眼
static CString GetGasTypeName( int type )
{
    CString str;
    switch( type )
    {
    case 1:
        str = _T( "两帮" );
        break;
    case 2:
        str = _T( "工作面" );
        break;
    case 3:
        str = _T( "开切眼" );
        break;
    }
    return str;
}

void PrintGasBoundary( AcGePoint3dArray& spts, AcGePoint3dArray& epts, AcGeDoubleArray& dirs, AcDbIntArray& gas_types )
{
    acutPrintf( _T( "\n打印瓦斯边界: " ) );

    int n = spts.length();
    for( int i = 0; i < n; i++ )
    {
        //acutPrintf(_T("\n  (%.3f, %.3f) --> (%.3f, %.3f)  方向: %.3f  类型: %s"),
        //	spts[i].x, spts[i].y, epts[i].x, epts[i].y, dirs[i], GetGasTypeName(gas_types[i]));

        acutPrintf( _T( "\n  (%.3f, %.3f) --> (%.3f, %.3f)  方向: %.3f  类型: %d" ),
                    spts[i].x, spts[i].y, epts[i].x, epts[i].y, dirs[i], gas_types[i] );
    }
    acutPrintf( _T( "\n" ) );
}