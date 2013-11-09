#include "StdAfx.h"

extern void FindAllGoafs( AcDbObjectIdArray& objIds );

/* ȫ�ֺ���(BuildGoafRegion.cpp) */
extern void GetGoafPolygon( const AcDbObjectId& objId, AcGePoint3dArray& polygon );

/* ȫ�ֺ���(DoubleLineHelper.cpp) */
extern void FilterLines( const AcDbVoidPtrArray& all_lines, AcDbVoidPtrArray& lines, bool findWS );
extern int FindLineByPoints( const AcDbVoidPtrArray& lines, const AcGePoint3d& spt, const AcGePoint3d& ept );
extern void AdjustLinePolygon( const AcDbVoidPtrArray& lines, const AcGePoint3dArray& polygon, AcGePoint3dArray& line_polygon, bool isInner );
extern void AdjustPointPolygon( const AcDbVoidPtrArray& lines, const AcGePoint3dArray& polygon, AcGePoint3dArray& ext_polygon, bool isInner );

/* ȫ�ֺ���(GeoTool.cpp) */
extern int ClockWise( const AcGePoint3dArray& polygon );
extern void PartitionPolygon( const AcGePoint3dArray& polygon, int k, AcDbIntArray& opposite, double c );
extern void CaclDirections( bool isClockWise, const AcGePoint3dArray& spts, const AcGePoint3dArray& epts, AcGeDoubleArray& dirs );
extern void FindPolygonColinearEdges( const AcGePoint3dArray& polygons, const AcDbIntArray& polygon_counts, AcDbIntArray& colinearEdges );

void BuildGoafPolygonArray( const AcDbObjectIdArray& objIds, AcGePoint3dArray& polygons, AcDbIntArray& polygon_counts )
{
    for( int i = 0; i < objIds.length(); i++ )
    {
        // ��ȡ�ɿ����Ķ����
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
        parTypes.append( 0 ); // ��ʼ��Ϊ0(��ʾ����״̬)
        if( FindLineByPoints( ws_lines, polygon[i], polygon[( i + 1 ) % n] ) != -1 )
        {
            ws_lines_pos.append( i );
        }
    }

    acutPrintf( _T( "\n�ɿ����Ϲ��������:%d" ), ws_lines_pos.length() );
    if( ws_lines_pos.isEmpty() ) return false;
    // ����ɿ����Ĺ����治�ܳ���2��
    if( ws_lines_pos.length() > 2 ) return false;
    // 2���������λ�ã�
    //		1) ����������
    //		2) �����滥Ϊ����
    //		3) �������

    // ��ʼ����������parTypes;
    //	1 -- ����
    //	2 -- ������
    //	3 -- ������
    for( int i = 0; i < n; i++ )
    {
        parTypes[i] = 1; // Ĭ��Ϊ����
    }
    for( int i = 0; i < ws_lines_pos.length(); i++ )
    {
        parTypes[ws_lines_pos[i]] = 2;
    }

    // �����ֻ��һ�����ƹ���ֵ
    // �ƽ����0.382/0.618��������������0.236/0.764��0.146/0.854��0.5/0.5
    // http://blog.eastmoney.com/junjun1997/blog_130654962.html
    const double c = 0.854;

    // ���ҹ����Ŀ�����
    // ʹ������cut_counts��Ƿ�֧����Ϊ�����۵Ĵ���
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
        // �������Ϊ�����۵Ĵ������ڹ�����ĸ���
        // ��÷�֧��Ϊ�����Ŀ�����
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

        // ���������չ������û�з����仯
        // ��ʹ������չ����
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

    // �����ɿ��������(����)
    // ��֤���Ǳ˴�������
    AcGePoint3dArray inner_polygon;
    AdjustPointPolygon( lines, goaf_polygon, inner_polygon, true );

    // �����ɿ��������(����)
    // ����֤���Ǳ˴�������
    AcGePoint3dArray outer_polygon;
    AdjustLinePolygon( lines, goaf_polygon, outer_polygon, false );

    // �ֽ�����Ϊֱ������
    ExplodeGoafPolygon( goaf_polygon, inner_polygon, outer_polygon, spts, epts );
    //ExplodeLinePolygon(outer_polygon, spts, epts);

    // ���㷽��(��ֱ��֧����ɿ�����)
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
    // �������вɿ����߶�Ӧ��ֱ��
    AcDbIntArray goaf_linePos;
    FindGoafPolygonLinePos( lines, polygons, polygon_counts, goaf_linePos );

    // �ֽ����еķ�֧
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

// ���Ҳɿ�����������
void FindGasBoundary( const AcDbObjectIdArray& objIds,
                      const AcDbVoidPtrArray& lines,
                      AcGePoint3dArray& spts,
                      AcGePoint3dArray& epts,
                      AcGeDoubleArray& dirs,
                      AcDbIntArray& gas_types,
                      AcDbObjectIdArray& gas_objIds )
{
    // �������еĲɿ���
    AcDbObjectIdArray goaf_objIds;
    FindAllGoafs( goaf_objIds );

    // ���ɿ��������ת����һ��1ά����
    AcGePoint3dArray polygons;
    AcDbIntArray polygon_counts;
    BuildGoafPolygonArray( goaf_objIds, polygons, polygon_counts );

    // ��ǲɿ�����֧�Ƿ��������ɿ����й��߱�
    AcDbIntArray colinearEdges;
    FindPolygonColinearEdges( polygons, polygon_counts, colinearEdges );

    // �������еĹ�����
    AcDbVoidPtrArray ws_lines;
    FilterLines( lines, ws_lines, true );

    // ���ֲɿ��������(�����桢���������)
    AcDbIntArray parTypes;
    PartitionGoafPolygons( ws_lines, polygons, polygon_counts, parTypes );

    assert( parTypes.length() == polygons.length() );

    // ��������Ҫ���⴦��
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

// ����Ӧ��1����������˹�߽�
// gas_type =
bool HasWSGasBoundary( const AcDbIntArray& gas_types )
{
    return gas_types.contains( 2 );
}

// ��ʼ����������parTypes;
//	1 -- ����
//	2 -- ������
//	3 -- ������
static CString GetGasTypeName( int type )
{
    CString str;
    switch( type )
    {
    case 1:
        str = _T( "����" );
        break;
    case 2:
        str = _T( "������" );
        break;
    case 3:
        str = _T( "������" );
        break;
    }
    return str;
}

void PrintGasBoundary( AcGePoint3dArray& spts, AcGePoint3dArray& epts, AcGeDoubleArray& dirs, AcDbIntArray& gas_types )
{
    acutPrintf( _T( "\n��ӡ��˹�߽�: " ) );

    int n = spts.length();
    for( int i = 0; i < n; i++ )
    {
        //acutPrintf(_T("\n  (%.3f, %.3f) --> (%.3f, %.3f)  ����: %.3f  ����: %s"),
        //	spts[i].x, spts[i].y, epts[i].x, epts[i].y, dirs[i], GetGasTypeName(gas_types[i]));

        acutPrintf( _T( "\n  (%.3f, %.3f) --> (%.3f, %.3f)  ����: %.3f  ����: %d" ),
                    spts[i].x, spts[i].y, epts[i].x, epts[i].y, dirs[i], gas_types[i] );
    }
    acutPrintf( _T( "\n" ) );
}