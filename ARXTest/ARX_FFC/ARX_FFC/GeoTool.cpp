#include "StdAfx.h"
#include "GeoDef.h"

#include <cmath>
#include <algorithm>

#define EPLISON 1e-5

#define NOMINMAX
#undef min
#undef max

// �ж϶���η���
int ClockWise( const AcGePoint3dArray& polygon )
{
    int n = polygon.length();
    if ( n < 3 ) return 0;

    int count = 0;
    for( int i = 0; i < n; i++ )
    {
        int j = ( i + 1 ) % n;
        int k = ( i + 2 ) % n;
        double z  = ( polygon[j].x - polygon[i].x ) * ( polygon[k].y - polygon[j].y );
        z -= ( polygon[j].y - polygon[i].y ) * ( polygon[k].x - polygon[j].x );
        if ( z < 0 )
        {
            count--;
        }
        else if ( z > 0 )
        {
            count++;
        }
    }
    if ( count > 0 )
        return -1;
    else if ( count < 0 )
        return 1;
    else
        return 0;
}

// �жϸ������Ƿ����
static bool IsDoubleEqual( double a, double b )
{
    return abs( a - b ) < EPLISON;
}

static bool IsPointEqual( const AcGePoint3d& p1, const AcGePoint3d& p2 )
{
    bool b1 = abs( p1.x - p2.x ) < EPLISON;
    bool b2 = abs( p1.y - p2.y ) < EPLISON;
    return ( b1 & b2 );
}

bool IsPointOnline( const AcGePoint3d& p1, const AcGePoint3d& p2, const AcGePoint3d& p )
{
    double x1, y1, x2, y2;
    x1 = p.x - p1.x;
    x2 = p2.x - p1.x;
    y1 = p.y - p1.y;
    y2 = p2.y - p1.y;
    if( !IsDoubleEqual( x1 * y2, x2 * y1 ) ) return false;

    return std::min( p1.x, p2.x ) <= p.x && p.x <= std::max( p1.x, p2.x ) &&
           std::min( p1.y, p2.y ) <= p.y && p.y <= std::max( p1.y, p2.y );
}

// �жϵ��Ƿ��ڶ������
bool IsPointInPolygon ( const AcGePoint3d& p, const AcGePoint3dArray& ptPolygon )
{
    int nCount = ptPolygon.length();
    int nCross = 0;
    for( int i = 0; i < nCount; i++ )
    {
        AcGePoint3d p1 = ptPolygon[i];
        AcGePoint3d p2 = ptPolygon[( i + 1 ) % nCount];
        if( IsPointOnline( p1, p2, p ) )
        {
            return true;
        }

        // ��� y=p.y �� p1p2 �Ľ���
        if ( p1.y == p2.y ) // p1p2 �� y=p0.yƽ��
            continue;
        if ( p.y < std::min( p1.y, p2.y ) ) // ������p1p2�ӳ�����
            continue;
        if ( p.y >= std::max( p1.y, p2.y ) ) // ������p1p2�ӳ�����
            continue;
        // �󽻵�� x ����
        double x = ( double )( p.y - p1.y ) * ( double )( p2.x - p1.x ) / ( double )( p2.y - p1.y ) + p1.x;

        if ( x > p.x )
            nCross++; // ֻͳ�Ƶ��߽���
    }
    // ���߽���Ϊż�������ڶ����֮��
    return ( nCross % 2 == 1 );
}

void BuildPolygonFromLines( const AcGePoint3dArray& spts, const AcGePoint3dArray& epts, AcGePoint3dArray& polygon )
{
    AcDbIntArray used;
    for( int i = 0; i < spts.length(); i++ )
    {
        used.append( 0 );
    }

    polygon.append( spts[0] );
    used[0] = 1;

    AcGePoint3d spt = spts[0];
    AcGePoint3d ept = epts[0];
    while( ept != spt )
    {
        spt = ept;
        bool ret = false;
        for( int i = 0; i < spts.length(); i++ )
        {
            if( used[i] == 1 ) continue;

            if( ept == spts[i] )
            {
                ept = epts[i];
                used[i] = 1;
                ret = true;
                break;
            }
            else if( ept == epts[i] )
            {
                ept = spts[i];
                used[i] = 1;
                ret = true;
                break;
            }
        }
        if( ret )
        {
            polygon.append( spt );
        }
    }
}

static AcGePoint3d LengthLine( const AcGePoint3d& spt, const AcGePoint3d& ept, double d )
{
    AcGeVector3d v = ept - spt;
    double L = v.length() + d;
    v.normalize();
    return spt + v * L;
}

int FindPointOnPolygon( const AcGePoint3d& pt, const AcGePoint3dArray& polygon )
{
    int pos = -1;
    int n = polygon.length();
    for( int i = 0; i < n; i++ )
    {
        AcGePoint3d spt = polygon[i], ept = polygon[( i + 1 ) % n];
        if( spt == pt )
        {
            pos = i;
            break;
        }
        else if( ept == pt )
        {
            pos = ( i + 1 ) % n;
            break;
        }
        else if( IsPointOnline( spt, ept, pt ) )
        {
            pos = i;
            break;
        }
    }
    return pos;
}

void FindLinesInPolygon( const AcGePoint3dArray& polygon, AcGePoint3dArray& spts, AcGePoint3dArray& epts )
{
    AcGePoint3dArray all_spts, all_epts;
    all_spts.append( spts );
    all_epts.append( epts );

    spts.removeAll();
    epts.removeAll();

    const double d = -0.1;

    for( int i = 0; i < all_spts.length(); i++ )
    {
        AcGePoint3d spt = all_spts[i], ept = all_epts[i];

        if( FindPointOnPolygon( spt, polygon ) != -1 )
        {
            spt = LengthLine( ept, spt, d );
        }

        if( FindPointOnPolygon( ept, polygon ) != -1 )
        {
            ept = LengthLine( spt, ept, d );
        }

        if( IsPointInPolygon( all_spts[i], polygon ) &&
                IsPointInPolygon( all_epts[i], polygon ) )
        {
            spts.append( all_spts[i] );
            epts.append( all_epts[i] );
        }
    }
}

void CaclLine( const AcGePoint3d& pt, double angle, double L, AcGePoint3d& spt, AcGePoint3d& ept )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );

    v.rotateBy( angle + PI * 0.5, AcGeVector3d::kZAxis );
    spt = pt + v * L * 0.5;

    v.rotateBy( PI, AcGeVector3d::kZAxis );
    ept = pt + v * L * 0.5;
}

static int FindMaxElementPos( const AcDbIntArray& datas )
{
    int pos = 0;
    int maxElement = -1;
    for( int i = 0; i < datas.length(); i++ )
    {
        if( maxElement < datas[i] )
        {
            maxElement = datas[i];
            pos = i;
        }
    }
    return pos;
}

static void FindContinuousEdges( int n, const AcDbIntArray& edges, AcDbIntArray& con_edges )
{
    // �������飬�����������ֵĳ���
    AcDbIntArray con_edges_len_array;

    int pos = edges[0];
    int count = 1;
    for( int i = 1; i < edges.length(); i++ )
    {
        if( edges[i] == ( pos + 1 ) % n )
        {
            count++;
        }
        else
        {
            con_edges_len_array.append( count );
            count = 1;
        }
        pos = edges[i];
    }
    con_edges_len_array.append( count );

    // ��������Ԫ��λ��
    // ������������֧���ĸ���
    int k = FindMaxElementPos( con_edges_len_array );

    int s = 0;
    for( int i = 0; i < k; i++ )
    {
        s += con_edges_len_array[i];
    }
    int t = s + con_edges_len_array[k];
    for( int i = s; i < t; i++ )
    {
        con_edges.append( edges[i] );
    }
}

static double FindMaxAngle( const AcGeDoubleArray& angles )
{
    double maxAngle = -1.0;
    for( int i = 0; i < angles.length(); i++ )
    {
        if( maxAngle < angles[i] )
        {
            maxAngle = angles[i];
        }
    }
    return maxAngle;
}

// ����һ������Σ�ָ��������ϵĵ�k����
// ���Ƽ�����ñߵ�"�����"
void PartitionPolygon( const AcGePoint3dArray& polygon, int k, AcDbIntArray& opposite, double c )
{
    int n = polygon.length();

    // ��ʼ������
    AcGeVector3d sv = polygon[( k + 1 ) % n] - polygon[k % n];

    // �����������ʼ�ߵĽǶ�
    AcGeDoubleArray angles;
    for( int i = 0; i < n; i++ )
    {
        AcGeVector3d v = polygon[( i + 1 ) % n] - polygon[i];
        angles.append( sv.angleTo( v ) );
    }

    // �������Ƕ�
    double maxAngle = FindMaxAngle( angles );
    acutPrintf( _T( "\n���Ƕ�:%.3f" ), maxAngle );

    // �ų��Ƕ�С��D�ķ�֧
    double D = c * maxAngle;

    AcDbIntArray edges;
    for( int i = 0; i < angles.length(); i++ )
    {
        acutPrintf( _T( "\n��%d���Ƕ�:%.3f\n" ), i, angles[i] );

        // �ų����ڵı�
        int d = abs( k - i );
        if( d == 1 || ( n - d ) == 1 ) continue;

        if( angles[i] > D )
        {
            edges.append( i );
        }
    }

    FindContinuousEdges( n, edges, opposite );
}

static bool IsColinearLine( const AcGePoint3d& p1, const AcGePoint3d& p2, const AcGePoint3d& p3, const AcGePoint3d& p4 )
{
    AcGeLine3d L1( p1, p2 ), L2( p3, p4 );
    return L1.isColinearTo( L2 );
}

static bool IsColinearEdge( const AcGePoint3dArray& polygon, const AcGePoint3d& spt, const AcGePoint3d& ept )
{
    bool ret = false;
    int n = polygon.length();
    for( int i = 0; i < n; i++ )
    {
        /*	acutPrintf(_T("�ж� \n"));
        	acutPrintf(_T("\t(%.3f, %.3f) -> (%.3f, %.3f) ��  \n"), spt.x, spt.y, ept.x, ept.y);
        	acutPrintf(_T("\t%.3f, %.3f) -> (%.3f, %.3f)�Ƿ���\n"), polygon[i].x, polygon[i].y, polygon[(i+1)%n].x, polygon[(i+1)%n].y);*/
        if( IsColinearLine( spt, ept, polygon[i], polygon[( i + 1 ) % n] ) )
        {
            acutPrintf( _T( "\t(%.3f, %.3f) -> (%.3f, %.3f) ��  \n" ), spt.x, spt.y, ept.x, ept.y );
            acutPrintf( _T( "\t%.3f, %.3f) -> (%.3f, %.3f)�Ƿ���\n" ), polygon[i].x, polygon[i].y, polygon[( i + 1 ) % n].x, polygon[( i + 1 ) % n].y );

            ret = true;
            break;
        }
    }
    return ret;
}

static bool IsColinearEdge( const AcGePoint3dArray& polygons,
                            const AcDbIntArray& polygon_counts,
                            int k,
                            const AcGePoint3d& spt,
                            const AcGePoint3d& ept )
{
    bool ret = false;

    int s = 0, t = 0;
    for( int i = 0; i < polygon_counts.length(); i++ )
    {
        t = s + polygon_counts[i];
        if( i != k )
        {
            AcGePoint3dArray goaf_polygon;
            for( int j = s; j < t; j++ )
            {
                goaf_polygon.append( polygons[j] );
            }

            if( IsColinearEdge( goaf_polygon, spt, ept ) )
            {
                ret = true;
                break;
            }
        }
        s = t;
    }
    return ret;
}

static void FindPolygonColinearEdges( const AcGePoint3dArray& polygons, const AcDbIntArray& polygon_counts, int k, AcDbIntArray& colinearEdges )
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

    int n = goaf_polygon.length();
    for( int i = 0; i < n; i++ )
    {
        AcGePoint3d spt = goaf_polygon[i], ept = goaf_polygon[( i + 1 ) % n];
        bool ret = IsColinearEdge( polygons, polygon_counts, k, spt, ept );
        colinearEdges.append( ret ? 1 : 0 );
    }
}

void FindPolygonColinearEdges( const AcGePoint3dArray& polygons, const AcDbIntArray& polygon_counts, AcDbIntArray& colinearEdges )
{
    // �ж�ÿ���ɿ����еı�������������Ƿ��й��߱�
    for( int i = 0; i < polygon_counts.length(); i++ )
    {
        AcDbIntArray goaf_colinearEdges;
        FindPolygonColinearEdges( polygons, polygon_counts, i, goaf_colinearEdges );
        colinearEdges.append( goaf_colinearEdges );
    }
}

void ExplodePointPolygon( const AcGePoint3dArray& polygon, AcGePoint3dArray& spts, AcGePoint3dArray& epts )
{
    int n = polygon.length();
    for( int i = 0; i < n; i++ )
    {
        spts.append( polygon[i] );
        epts.append( polygon[( i + 1 ) % n] );
    }
}

void ExplodeLinePolygon( const AcGePoint3dArray& line_polygon, AcGePoint3dArray& spts, AcGePoint3dArray& epts )
{
    int n = line_polygon.length() / 2;
    for( int i = 0; i < n; i++ )
    {
        spts.append( line_polygon[2 * i] );
        epts.append( line_polygon[2 * i + 1] );
    }
}

double LinePerpendicularAngle( const AcGePoint3d& spt, const AcGePoint3d& ept, bool isClockWise )
{
    AcGeVector3d v = ept - spt;
    if( isClockWise )
    {
        v.rotateBy( 0.5 * PI, AcGeVector3d::kZAxis );
    }
    else
    {
        v.rotateBy( -0.5 * PI, AcGeVector3d::kZAxis );
    }
    return v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
}

void CaclDirections( bool isClockWise, const AcGePoint3dArray& spts, const AcGePoint3dArray& epts, AcGeDoubleArray& dirs )
{
    int n = spts.length();
    for( int i = 0; i < n; i++ )
    {
        dirs.append( LinePerpendicularAngle( spts[i], epts[i], isClockWise ) );
    }
}

double PolygonArea( const AcGePoint3dArray& polygon )
{
    int n = polygon.length();
    if( n < 3 ) return 0;

    double s = polygon[0].y * ( polygon[n - 1].x - polygon[1].x );
    for( int i = 1; i < n; i++ )
    {
        s += polygon[i].y * ( polygon[( i - 1 )].x - polygon[( i + 1 ) % n].x );
    }
    return abs( s * 0.5 );
}