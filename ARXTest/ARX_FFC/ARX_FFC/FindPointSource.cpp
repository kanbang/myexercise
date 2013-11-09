#include "StdAfx.h"

#include "GeoDef.h"

/* ȫ�ֺ���(RegionHelper.cpp) */
extern AcDbRegion* BuildPointPolygonRegion( const AcGePoint3dArray& polygon );

/* ȫ�ֺ���(DoubleLineHelper.cpp) */
extern void AdjustPointPolygon( const AcDbVoidPtrArray& lines, const AcGePoint3dArray& polygon, AcGePoint3dArray& ext_polygon, bool isInner );

/* ȫ�ֺ���(Tool.cpp) */
extern void FindAllGoafs( AcDbObjectIdArray& objIds );
extern void GetGoafPolygon( const AcDbObjectId& objId, AcGePoint3dArray& polygon );
extern void FindAllAirLeaks( AcDbObjectIdArray& objIds );
extern void FindAllGasPipes( AcDbObjectIdArray& objIds );
extern void FindAllNitrogenPipes( AcDbObjectIdArray& objIds );
extern void GetAirLeakPoints( AcDbObjectIdArray& objIds, AcGePoint3dArray& pts );
extern void GetGasPipePoints( AcDbObjectIdArray& objIds, AcGePoint3dArray& pts );
extern void GetNitrogenPipePoints( AcDbObjectIdArray& objIds, AcGePoint3dArray& pts );

/* ȫ�ֺ���(GeoTool.cpp) */
extern int FindPointOnPolygon( const AcGePoint3d& pt, const AcGePoint3dArray& polygon );
extern bool IsPointInPolygon ( const AcGePoint3d& p, const AcGePoint3dArray& ptPolygon );
extern void BuildGoafPolygonArray( const AcDbObjectIdArray& objIds, AcGePoint3dArray& polygons, AcDbIntArray& polygon_counts );
extern int ClockWise( const AcGePoint3dArray& polygon );

static AcGePoint3d ProjectPointOfTwoLine( const AcGePoint3d& spt1, const AcGePoint3d& ept1,
        const AcGePoint3d& spt2, const AcGePoint3d& ept2,
        const AcGePoint3d& pt )
{
    // �������ֱ��spt1->ept1�ϵı�����ϵ
    double c = pt.distanceTo( spt1 ) / ept1.distanceTo( spt1 );

    acutPrintf( _T( "\n����c:%.4f" ), c );
    acutPrintf( _T( "\nspt->(%.3f, %.3f), ept->(%.3f, %.3f)" ), spt2.x, spt2.y, ept2.x, ept2.y );
    AcGeVector3d v = ept2 - spt2;
    return spt2 + v * c;
}

static AcGePoint3d MinorAjustPointSource( bool isClockWise, const AcGePoint3d& p1, const AcGePoint3d& p2, const AcGePoint3d& p3, const AcGePoint3d& pt )
{
    acutPrintf( _T( "\np1=(%.3f, %.3f), p2=(%.3f, %.3f), p2=(%.3f, %.3f)" ), p1.x, p1.y, p2.x, p2.y, p3.x, p3.y );
    AcGeVector3d v1 = p2 - p1, v2 = p3 - p2;
    v1.normalize();
    v2.normalize();

    double angle = ( isClockWise ? 0.5 * PI : -0.5 * PI );
    v1.rotateBy( angle, AcGeVector3d::kZAxis );
    v2.rotateBy( angle, AcGeVector3d::kZAxis );

    AcGeVector3d v3 = v1 + v2;
    v3.normalize();

    return pt + v3 * 0.1;
}

static void AdjustObturationPointSource( const AcGePoint3dArray& polygon, const AcGePoint3dArray& ext_polygon, AcGePoint3dArray& ob_pts )
{
    bool isClockWise = ( ClockWise( polygon ) == -1 );

    int n = ob_pts.length();
    int m = polygon.length();
    for( int i = 0; i < n; i++ )
    {
        int pos = FindPointOnPolygon( ob_pts[i], polygon );
        //acutPrintf(_T("\n��Դλ��:%d"), pos);
        if( pos != -1 )
        {
            int p1 = ( ( pos == 0 ) ? m - 1 : pos - 1 );
            int p2 = pos;
            int p3 = ( ( pos == m - 1 ) ? 0 : pos + 1 );
            ob_pts[i] = ProjectPointOfTwoLine(
                            polygon[p2],
                            polygon[p3],
                            ext_polygon[p2],
                            ext_polygon[p3],
                            ob_pts[i] );

            // ���н���Դ������ɿ�����΢С�ĵ���
            ob_pts[i] = MinorAjustPointSource(
                            isClockWise,
                            polygon[p1],  // ǰһ����
                            polygon[p2],            // ��ǰ��
                            polygon[p3],      // ��һ����
                            ob_pts[i] );
        }
    }
}

// ���ݲɿ������¼����ܱյĵ�Դ����
void AdjustObturationPointSource( const AcDbVoidPtrArray& lines, AcGePoint3dArray& ob_pts )
{
    // �������еĲɿ���
    AcDbObjectIdArray objIds;
    FindAllGoafs( objIds );

    for( int i = 0; i < objIds.length(); i++ )
    {
        // ��ȡ�ɿ����Ķ����
        AcGePoint3dArray polygon;
        GetGoafPolygon( objIds[i], polygon );

        // �����ɿ��������(������չ)
        AcGePoint3dArray ext_polygon;
        AdjustPointPolygon( lines, polygon, ext_polygon, true );

        // �����Դ��������
        AdjustObturationPointSource( polygon, ext_polygon, ob_pts );
    }
}

static bool IsPointInGoafPolygon( const AcGePoint3dArray& polygons, const AcDbIntArray& polygon_counts, int k, const AcGePoint3d& pt )
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
    return IsPointInPolygon( pt, polygon );
}

static bool IsPointInGoafPolygon( const AcGePoint3dArray& polygons, const AcDbIntArray& polygon_counts, const AcGePoint3d& pt )
{
    bool ret = false;
    for( int i = 0; i < polygon_counts.length(); i++ )
    {
        if( IsPointInGoafPolygon( polygons, polygon_counts, i, pt ) )
        {
            ret = true;
            break;
        }
    }
    return ret;
}

enum POINT_SOURCE_TYPE
{
    PST_AIR_LEAK = 1,  // ©��Դ��
    PST_GAS_PIPE = 2,  // ��˹���
    PST_N2_PIPE  = 3   // ע����
};

static void BuildObjectIdAndPointArray( POINT_SOURCE_TYPE type, AcDbObjectIdArray& all_objIds, AcGePoint3dArray& all_pts )
{
    if( type == PST_AIR_LEAK )
    {
        FindAllAirLeaks( all_objIds );
        GetAirLeakPoints( all_objIds, all_pts );
    }
    else if( type == PST_GAS_PIPE )
    {
        FindAllGasPipes( all_objIds );
        GetGasPipePoints( all_objIds, all_pts );
    }
    else if( type == PST_N2_PIPE )
    {
        FindAllNitrogenPipes( all_objIds );
        GetNitrogenPipePoints( all_objIds, all_pts );
    }
}

static void FindPointSource_Impl( const AcGePoint3dArray& polygons, const AcDbIntArray& polygon_counts, POINT_SOURCE_TYPE type, AcDbObjectIdArray& objIds, AcGePoint3dArray& pts )
{
    AcDbObjectIdArray all_objIds;
    AcGePoint3dArray all_pts;
    BuildObjectIdAndPointArray( type, all_objIds, all_pts );

    // �ų����ڲɿ����ڵ�Դ��
    for( int i = 0; i < all_pts.length(); i++ )
    {
        if( IsPointInGoafPolygon( polygons, polygon_counts, all_pts[i] ) )
        {
            objIds.append( all_objIds[i] );
            pts.append( all_pts[i] );
        }
    }
}

static void FindPointSource_Helper( POINT_SOURCE_TYPE type, AcDbObjectIdArray& objIds, AcGePoint3dArray& pts )
{
    // �������еĲɿ���
    AcDbObjectIdArray goaf_objIds;
    FindAllGoafs( goaf_objIds );

    // �ֽ�ɿ���Ϊ����Σ�����1ά����
    AcGePoint3dArray polygons;
    AcDbIntArray polygon_counts;
    BuildGoafPolygonArray( goaf_objIds, polygons, polygon_counts );

    FindPointSource_Impl( polygons, polygon_counts, type, objIds, pts );
}

void FindAirLeakPointSource( AcDbObjectIdArray& al_objIds, AcGePoint3dArray& al_pts )
{
    FindPointSource_Helper( PST_AIR_LEAK, al_objIds, al_pts );
}

void FindGasPipePointSource( AcDbObjectIdArray& gas_pipe_objIds, AcGePoint3dArray& gas_pipe_pts )
{
    FindPointSource_Helper( PST_GAS_PIPE, gas_pipe_objIds, gas_pipe_pts );
}

void FindNitrogenPipePointSource( AcDbObjectIdArray& n2_pipe_objIds, AcGePoint3dArray& n2_pipe_pts )
{
    FindPointSource_Helper( PST_N2_PIPE, n2_pipe_objIds, n2_pipe_pts );
}