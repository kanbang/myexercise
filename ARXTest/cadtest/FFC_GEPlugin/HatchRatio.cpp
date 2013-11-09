#include "StdAfx.h"

#define PI 3.1415926535897932384626433832795

static double PolygonMaxDistanceEdge( const AcGePoint3dArray& polygon )
{
    double maxDist = polygon[0].distanceTo( polygon[1] );

    int n = polygon.length();
    for( int i = 1; i < n; i++ )
    {
        double dist = polygon[i].distanceTo( polygon[( i + 1 ) % n] );
        if( dist > maxDist )
        {
            maxDist = dist;
        }
    }
    return maxDist;
}

static double PolygonArea( const AcGePoint3dArray& polygon )
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

static double Circumference( const AcGePoint3dArray& polygon )
{
    double c = 0;

    int n = polygon.length();
    for( int i = 0; i < n; i++ )
    {
        c += polygon[i].distanceTo( polygon[( i + 1 ) % n] );
    }
    return c;
}

static int PolyEdgeNum( const AcGePoint3dArray& polygon )
{
    return polygon.length();
}

double CaclHatchRatio( const AcGePoint3dArray& polygon )
{
    double scale = 1.0;

    // ����αߵĸ���
    int N = PolyEdgeNum( polygon );

    // ��ȡ���
    double maxDist = PolygonMaxDistanceEdge( polygon );

    // �������
    double S = PolygonArea( polygon );

    // �����ܳ�
    double C = Circumference( polygon );

    // �ƽ����
    const double phi = 0.61803398827239717009900391523914;

    //acutPrintf(_T("\nN:%d\nmaxDis:%.3f\nS:%.3f\nC:%.3f"), N, maxDist, S, C);
    // ʹ�����¹�ʽ����scale
    // scale = (Dmax*N/C)*(1/(10*phi*pi))*(S/C)
    // �ַܷ�Ϊ3�
    //		1) ��������/���ܳ�ƽ��ֵ�ı�ֵ
    //		2) �ƽ���ʳ���Բ���ʵĵ�����1/10
    //		3) ������ܳ��ı�ֵ
    scale = ( maxDist * N / C ) * S / ( 10 * phi * PI * C );

    if( scale < 0 )
    {
        scale = 1.0;
    }
    return scale;
}