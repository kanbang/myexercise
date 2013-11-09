// 参考
// http://dev.gameres.com/Program/Abstract/Geometry.htm

#include "GeoAlgo.h"
#include <cmath>

#define EPLISON 1e-4
#define PI 3.1415926535897932384626433832795

// 判断浮点数是否相等
bool IsDoubleEqual( double a, double b )
{
    return abs( a - b ) < EPLISON;
}

bool IsLessEqual( double a, double b )
{
    return ( a - b ) < EPLISON;
}

// 判断是否有等值点
// 1 -- 有  -1) 没有 0 -- 与z1或z2相等
int InRange( double z1, double z2, double z )
{
    double d = ( z - z1 ) * ( z - z2 );
    if( d < 0 ) return 1;
    if( d > 0 ) return -1;
    return 0;
}

bool IsPointEqual( const DT_Point& p1, const DT_Point& p2 )
{
    bool b1 = abs( p1.x - p2.x ) < EPLISON;
    bool b2 = abs( p1.y - p2.y ) < EPLISON;
    return ( b1 & b2 );
}

// 线性插值
void LinearInterpolate( const DT_Point& p1, const DT_Point& p2, DT_Point& p )
{
    double c = ( p.z - p1.z ) / ( p2.z - p1.z );
    p.x = p1.x + ( p2.x - p1.x ) * c;
    p.y = p1.y + ( p2.y - p1.y ) * c;
}

bool IsPointOnline( const DT_Point& p1, const DT_Point& p2, const DT_Point& p )
{
    double x1, y1, x2, y2;
    x1 = p.x - p1.x;
    x2 = p2.x - p1.x;
    y1 = p.y - p1.y;
    y2 = p2.y - p1.y;
    if( !IsDoubleEqual( x1 * y2, x2 * y1 ) ) return false;

    return IsLessEqual( std::min( p1.x, p2.x ), p.x ) && IsLessEqual( p.x, std::max( p1.x, p2.x ) ) &&
           IsLessEqual( std::min( p1.y, p2.y ), p.y ) && IsLessEqual( p.y, std::max( p1.y, p2.y ) );
}

// 功能：判断点是否在多边形内
// 方法：求解通过该点的水平线与多边形各边的交点
// 结论：单边交点为奇数，成立!
//参数：
// DT_Point p 指定的某个点
// LPPOINT ptPolygon 多边形的各个顶点坐标（首末点可以不一致）
// int nCount 多边形定点的个数
bool IsPointInPolygon ( const DT_Point& p, const PointArray& ptPolygon )
{
    int nCount = ptPolygon.size();
    int nCross = 0;
    for( int i = 0; i < nCount; i++ )
    {
        DT_Point p1 = ptPolygon[i];
        DT_Point p2 = ptPolygon[( i + 1 ) % nCount];
        if( IsPointOnline( p1, p2, p ) )
        {
            return true;
        }

        // 求解 y=p.y 与 p1p2 的交点
        if ( p1.y == p2.y ) // p1p2 与 y=p0.y平行
            continue;
        if ( p.y < std::min( p1.y, p2.y ) ) // 交点在p1p2延长线上
            continue;
        if ( p.y >= std::max( p1.y, p2.y ) ) // 交点在p1p2延长线上
            continue;
        // 求交点的 x 坐标
        double x = ( double )( p.y - p1.y ) * ( double )( p2.x - p1.x ) / ( double )( p2.y - p1.y ) + p1.x;

        if ( x > p.x )
            nCross++; // 只统计单边交点
    }
    // 单边交点为偶数，点在多边形之外
    return ( nCross % 2 == 1 );
}

double GetPointDistanceToLine( const DT_Point& p, const DT_Point& p1, const DT_Point& p2 )
{
    if ( p1.x == p2.x )
    {
        return abs( p1.y - p2.y );
    }
    if ( p1.y == p2.y )
    {
        return abs( p1.x - p2.x );
    }
    double k = ( p2.y - p1.y ) / ( p2.x - p1.x );
    double c = ( p2.x * p1.y - p1.x * p2.y ) / ( p2.x - p1.x );
    return abs( ( k * p.x - p.y + c ) / sqrt( k * k + 1 ) );
}

double Distance( const DT_Point& p1, const DT_Point& p2 )
{
    return sqrt( ( p1.x - p2.x ) * ( p1.x - p2.x ) + ( p1.y - p2.y ) * ( p1.y - p2.y ) );
}

double Distance_2( const DT_Point& p1, const DT_Point& p2 )
{
    return ( p1.x - p2.x ) * ( p1.x - p2.x ) + ( p1.y - p2.y ) * ( p1.y - p2.y );
}

DT_Point MidPoint( const DT_Point& p1, const DT_Point& p2 )
{
    DT_Point p;
    p.x = ( p1.x + p2.x ) * 0.5;
    p.y = ( p1.y + p2.y ) * 0.5;
    return p;
}

double Direction( const DT_Point& p1, const DT_Point& p2 )
{
    // 不考虑dx和dy同时等于0的情况
    double dx = p2.x - p1.x, dy = p2.y - p1.y;
    if( abs( dx ) < EPLISON )
    {
        if( dy > 0 )
            return 0.5 * PI;
        else
            return -0.5 * PI;
    }
    if( abs( dy ) < EPLISON )
    {
        if( dx > 0 )
            return 0;
        else
            return PI;
    }
    return atan( ( p2.y - p1.y ) / ( p2.x - p1.x ) );
}

double PolygonArea( const PointArray& polygon )
{
    int n = polygon.size();
    if( n < 3 ) return 0;

    double s = polygon[0].y * ( polygon[n - 1].x - polygon[1].x );
    for( int i = 1; i < n; i++ )
    {
        s += polygon[i].y * ( polygon[( i - 1 )].x - polygon[( i + 1 ) % n].x );
    }
    return s * 0.5;
}

DT_Point PolygonCentroid( const PointArray& polygon )
{
    double A = abs( PolygonArea( polygon ) );

    double sumx = 0, sumy = 0;
    int n = polygon.size();
    for( int i = 0; i < n; i++ )
    {
        double x1 = polygon[i].x;
        double y1 = polygon[i].y;
        double x2 = polygon[( i + 1 ) % n].x;
        double y2 = polygon[( i + 1 ) % n].y;

        sumx += ( x1 + x2 ) * ( x1 * y2 - x2 * y1 );
        sumy += ( y1 + y2 ) * ( x1 * y2 - x2 * y1 );
    }

    DT_Point p;
    p.x = sumx / ( 6 * A );
    p.y = sumy / ( 6 * A );
    p.z = 0;
    return p;
}

// 参考：http://hi.baidu.com/cityhacker/blog/item/ad9e1b82aba5cea50cf4d2a7.html
int ClockWise( const PointArray& polygon )
{
    int count = 0;

    int n = polygon.size();
    if ( n < 3 ) return 0;

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

    if( count > 0 )
        return -1;
    else if( count < 0 )
        return 1;
    else
        return 0;
}

int LineIntersect( const DT_Point& a, const DT_Point& b, const DT_Point& c, const DT_Point& d, DT_Point& p )
{
    double v = std::abs( b.y - a.y ) + std::abs( b.x - a.x ) + std::abs( d.y - c.y ) + std::abs( d.x - c.x );

    if( IsDoubleEqual( v, 0 ) )
    {
        v = ( c.x - a.x ) + ( c.y - a.y );
        if( IsDoubleEqual( v, 0 ) )
        {
            //System.out.println("ABCD是同一个点！");
        }
        else
        {
            //System.out.println("AB是一个点，CD是一个点，且AC不同！");
        }
        return 0;
    }

    v = std::abs( b.y - a.y ) + std::abs( b.x - a.x );
    if( IsDoubleEqual( v, 0 ) )
    {
        v = ( a.x - d.x ) * ( c.y - d.y ) - ( a.y - d.y ) * ( c.x - d.x );
        if( IsDoubleEqual( v, 0 ) )
        {
            //System.out.println("A、B是一个点，且在CD线段上！");
        }
        else
        {
            //System.out.println("A、B是一个点，且不在CD线段上！");
        }
        return 0;
    }

    v = std::abs( d.y - c.y ) + std::abs( d.x - c.x );
    if( IsDoubleEqual( v, 0 ) )
    {
        v = ( d.x - b.x ) * ( a.y - b.y ) - ( d.y - b.y ) * ( a.x - b.x );
        if( IsDoubleEqual( v, 0 ) )
        {
            //System.out.println("C、D是一个点，且在AB线段上！");
        }
        else
        {
            //System.out.println("C、D是一个点，且不在AB线段上！");
        }
        return 0;
    }

    v = ( b.y - a.y ) * ( c.x - d.x ) - ( b.x - a.x ) * ( c.y - d.y );
    if( IsDoubleEqual( v, 0 ) )
    {
        //System.out.println("线段平行，无交点！");
        return 0;
    }

    p.x = ( ( b.x - a.x ) * ( c.x - d.x ) * ( c.y - a.y ) -
            c.x * ( b.x - a.x ) * ( c.y - d.y ) + a.x * ( b.y - a.y ) * ( c.x - d.x ) ) /
          ( ( b.y - a.y ) * ( c.x - d.x ) - ( b.x - a.x ) * ( c.y - d.y ) );
    p.y = ( ( b.y - a.y ) * ( c.y - d.y ) * ( c.x - a.x ) - c.y
            * ( b.y - a.y ) * ( c.x - d.x ) + a.y * ( b.x - a.x ) * ( c.y - d.y ) )
          / ( ( b.x - a.x ) * ( c.y - d.y ) - ( b.y - a.y ) * ( c.x - d.x ) );

    if( IsLessEqual( ( p.x - a.x ) * ( p.x - b.x ), 0 )
            && IsLessEqual( ( p.x - c.x ) * ( p.x - d.x ), 0 )
            && IsLessEqual( ( p.y - a.y ) * ( p.y - b.y ), 0 )
            && IsLessEqual( ( p.y - c.y ) * ( p.y - d.y ), 0 ) )
    {

        //System.out.println("线段相交于点(" + p.x + "," + p.y + ")！");
        return 1; // '相交
    }
    else
    {
        //System.out.println("线段相交于虚交点(" + p.x + "," + p.y + ")！");
        return -1; // '相交但不在线段上
    }
}

//bool LinesegInsidePolygon(const PointArray& polygon, const DT_Point& s, const DT_Point& t)
//{
//	// 判断线端l的端点是否不都在多边形内
//	if(!IsPointInPolygon(polygon, s) || !IsPointInPolygon(polygon, t)) return false;
//
//	int top=0, i, j;
//
//	PointArray PointSet;
//	DT_Point tmp;
//
//	int n = polygon.size();
//	for(i=0;i<n;i++)
//	{
//		// 线段s=(si, ti)
//		DT_Point si = polygon[i];
//		DT_Point ti = polygon[(i+1)%n];
//
//		if(IsPointOnline(si, ti, s)) //线段l的起始端点在线段s上
//			PointSet[top++] = s;
//
//		else if(IsPointOnline(si, ti, t)) //线段l的终止端点在线段s上
//			PointSet[top++] = t;
//		else
//		{
//			if(IsPointOnline(s, t, si)) //线段s的起始端点在线段l上
//				PointSet[top++] = si;
//			else if(IsPointOnline(s, t, ti)) // 线段s的终止端点在线段l上
//				PointSet[top++] = ti;
//			else
//			{
//				if(intersect(l,s)) // 这个时候如果相交，肯定是内交，返回false
//
//					return false;
//
//			}
//
//		}
//
//	}
//
//
//
//	for(i=0;i<top-1;i++) /* 冒泡排序，x坐标小的排在前面；x坐标相同者，y坐标小的排在前面*/
//
//	{
//
//		for(j=i+1;j<top;j++)
//
//		{
//
//			if( PointSet[i].x>PointSet[j].x ||fabs(PointSet[i].x-PointSet[j].x)<EP &&PointSet[i].y>PointSet[j].y )
//
//			{
//
//				tmp=PointSet[i];
//
//				PointSet[i]=PointSet[j];
//
//				PointSet[j]=tmp;
//
//			}
//
//		}
//
//	}
//
//
//
//	for(i=0;i<top-1;i++)
//
//	{
//
//		tmp.x=(PointSet[i].x+PointSet[i+1].x)/2; //得到两个相邻交点的中点
//
//		tmp.y=(PointSet[i].y+PointSet[i+1].y)/2;
//
//		if(!insidepolygon(n,polygon,tmp))
//
//			return false;
//
//	}
//
//	return true;
//
//}