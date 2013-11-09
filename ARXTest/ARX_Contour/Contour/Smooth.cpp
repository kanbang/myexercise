// �������⣬�ṩ�˶���spline��ֵ����
// 1) http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Interpolating_a_data_set
// 2) http://msdn.microsoft.com/zh-cn/library/4cf6we5y(v=vs.80).aspx
// ������ʹ�����е�cardinal spline(��������������3��Hermite��ֵ)
// google����"c++ spline"���кܶ�Ŀ������

// ������ʹ�õĿ�libspline
// http://developer.berlios.de/projects/libspline/

// http://hi.baidu.com/kondio_learning100/blog/item/7a17f4267b5c6109918f9df9.html/cmtid/3a84f2398c18fd2d97ddd880
// �����������һ�����򣬸о�������Ч��������̫�ã�����gdi+��Ч���Ƚ�(��������)

// http://blog.csdn.net/coy_wang/article/details/5086309
// "���⻬��", û���ҵ����еĳ���

#include "Contour.h"
#include "GeoAlgo.h"
#include "SplineWrapper.h"

#if _DEBUG
#pragma comment(lib, "libspline_d")
#else
#pragma comment(lib, "libspline")
#endif

static DT_Point Position2DT_Point( const Position& pos )
{
    DT_Point p = {pos.x, pos.y, 0};
    return p;
}

static void SplineInterpolate( const PointArray& inPts, PointArray& outPts, unsigned int N )
{
    int point_num = inPts.size();

    // �պ�
    bool isClose = IsPointEqual( inPts.back(), inPts.front() );
    if( isClose )
    {
        point_num--;
    }

    // Creates all control points to create the curve of
    Spline::ControlPoint* points = new Spline::ControlPoint[point_num];
    for( int i = 0; i < point_num; i++ )
    {
        points[i].position.x = ( float )inPts[i].x;
        points[i].position.y = ( float )inPts[i].y;
        points[i].position.z = 0.0f;
    }

    // Creates the curve. First argument is a pointer to the control points.
    // Second argument is the number of control points. The third is the curve type.
    // ע��ʹ��cardinal spline������ΪCARDINAL(Ĭ��ϵ��Ϊ1)�� CATMULLROM(�Զ�����ϵ��)
    SplineWrapper* curve = new SplineWrapper( points, point_num, CATMULLROM );

    // ƫ����(�������Ŵ�1��������ϸ��һЩ)
    //const int N = 1;
    const float C = 0.8f;
    float d = curve->getLength() / ( point_num * N - 1 );
    // The SplineWrapper has a position which is a value between 0 and the total length
    // of the curve. With the move(...) function you can increase or decrease this value
    // to move on the curve. The getPosition() function returns the world position
    // depending on how far you have moved on the curve.
    while( !curve->isEndReached() )
    {
        // Get the world position
        Position pos = curve->getPosition();
        outPts.push_back( Position2DT_Point( pos ) );

        // Travel 0.5 units on the curve each iteration
        curve->move( d );
    }
    outPts.push_back( inPts[point_num - 1] );
    delete curve;

    // ɾ���������ڴ�
    delete [] points;
    points = 0;

    if( isClose )
    {
        outPts.push_back( inPts.front() );
    }
}

void SmoothPoints( const PointArray& inPts, PointArray& outPts, unsigned int N )
{
    if( inPts.empty() ) return;
    SplineInterpolate( inPts, outPts, N );
}