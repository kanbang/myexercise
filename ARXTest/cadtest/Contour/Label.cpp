// �ο�����:
// ��ֵ�߱�ע��һ���㷨̽��
// http://www.cnblogs.com/think8848/archive/2011/05/04/2036179.html
// http://en.wikipedia.org/wiki/Automatic_label_placement

// Ŀǰʵ����2�ֱ�ע�㷨
// ����ȡ�������ķ���: ��ֵ�߶��������ߵ��е���Ϊ��עλ��

#include "Contour.h"
#include "GeoTool.h"

#include <algorithm>
#include <iterator>

static void UpdateIndexes( IntArray& indexes )
{
    IntArray t_indexes;
    for( int i = 0; i < ( int )indexes.size(); i++ )
    {
        if( indexes[i] != -1 ) t_indexes.push_back( indexes[i] );
    }
    indexes.clear();
    std::copy( t_indexes.begin(), t_indexes.end(), std::back_inserter( indexes ) );
}

static void ApproximatePolygon( const PointArray& cnpts, PointArray& polygon, double tolerance )
{
    if( cnpts.empty() ) return;

    int n = cnpts.size();
    // �Ƿ��յ�ֵ��???
    if( IsPointEqual( cnpts.front(), cnpts.back() ) )
    {
        n--;
    }
    int minEdge = n + 1, k = n;

    IntArray indexes;
    for( int i = 0; i < ( int )cnpts.size(); i++ )
    {
        indexes.push_back( i );
    }
    while( k < minEdge )
    {
        minEdge = k;
        int p = 0;
        while( p < minEdge - ( n % 2 == 0 ? 3 : 2 ) )
        {
            double distance = GetPointDistanceToLine( cnpts[indexes[p + 1]], cnpts[indexes[p]], cnpts[indexes[p + 2]] );
            if ( distance < tolerance )
            {
                indexes[p + 1] = -1;
                k -= 1;
            }
            p += 2;
        }
        UpdateIndexes( indexes );
    }

    for( int i = 0; i < ( int )indexes.size(); i++ )
    {
        polygon.push_back( cnpts[indexes[i]] );
    }
}

void SearchLabelPostion( const PointArray& cnpts, double tolerance, double width, LabelArray& la )
{
    // ������ƶ����
    PointArray polygon;
    ApproximatePolygon( cnpts, polygon, tolerance );

    double c = 0.85;
    // �ڶ�����ϲ��ұ�ע��
    for( int i = 0; i < ( int )polygon.size() - 1; i++ )
    {
        DT_Point p1 = polygon[i], p2 = polygon[i + 1];
        if( Distance( p1, p2 )*c > width )
        {
            DT_Label label = {MidPoint( p1, p2 ), Direction( p1, p2 )};
            la.push_back( label );
        }
    }
}

// ���ý��ƹ��Ƶ��㷨
// ����Ǳպϵ�ֵ�ߣ��������������������ı�
// ����ǿ��ŵ�ֵ�ߣ�������20%--80%��Χ�ڵĶ���������
void SearchLabelPostion( const PointArray& cnpts, DT_Point& tpt, double& angle )
{
    if( cnpts.empty() ) return;

    // ͨ������£���ֵ��������3����
    int s = 0, t = cnpts.size();
    if( !IsPointEqual( cnpts.front(), cnpts.back() ) )
    {
        s = cnpts.size() / 5;
        t = cnpts.size() - s;
    }

    int pos = s;
    double maxDist = Distance_2( cnpts[0], cnpts[1] );
    for( int i = s; i < t - 1; i++ )
    {
        double dist = Distance_2( cnpts[i], cnpts[i + 1] );
        if( dist > maxDist )
        {
            pos = i;
            maxDist = dist;
        }
    }
    tpt = MidPoint( cnpts[pos], cnpts[pos + 1] );
    angle = Direction( cnpts[pos], cnpts[pos + 1] );
}