// �ο�
// http://dev.gameres.com/Program/Abstract/Geometry.htm

#include "GeoAlgo.h"
#include <cmath>

#define EPLISON 1e-4
#define PI 3.1415926535897932384626433832795

// �жϸ������Ƿ����
bool IsDoubleEqual( double a, double b )
{
    return abs( a - b ) < EPLISON;
}

bool IsLessEqual( double a, double b )
{
    return ( a - b ) < EPLISON;
}

// �ж��Ƿ��е�ֵ��
// 1 -- ��  -1) û�� 0 -- ��z1��z2���
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

// ���Բ�ֵ
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

// ���ܣ��жϵ��Ƿ��ڶ������
// ���������ͨ���õ��ˮƽ�������θ��ߵĽ���
// ���ۣ����߽���Ϊ����������!
//������
// DT_Point p ָ����ĳ����
// LPPOINT ptPolygon ����εĸ����������꣨��ĩ����Բ�һ�£�
// int nCount ����ζ���ĸ���
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
    // ������dx��dyͬʱ����0�����
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

// �ο���http://hi.baidu.com/cityhacker/blog/item/ad9e1b82aba5cea50cf4d2a7.html
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
            //System.out.println("ABCD��ͬһ���㣡");
        }
        else
        {
            //System.out.println("AB��һ���㣬CD��һ���㣬��AC��ͬ��");
        }
        return 0;
    }

    v = std::abs( b.y - a.y ) + std::abs( b.x - a.x );
    if( IsDoubleEqual( v, 0 ) )
    {
        v = ( a.x - d.x ) * ( c.y - d.y ) - ( a.y - d.y ) * ( c.x - d.x );
        if( IsDoubleEqual( v, 0 ) )
        {
            //System.out.println("A��B��һ���㣬����CD�߶��ϣ�");
        }
        else
        {
            //System.out.println("A��B��һ���㣬�Ҳ���CD�߶��ϣ�");
        }
        return 0;
    }

    v = std::abs( d.y - c.y ) + std::abs( d.x - c.x );
    if( IsDoubleEqual( v, 0 ) )
    {
        v = ( d.x - b.x ) * ( a.y - b.y ) - ( d.y - b.y ) * ( a.x - b.x );
        if( IsDoubleEqual( v, 0 ) )
        {
            //System.out.println("C��D��һ���㣬����AB�߶��ϣ�");
        }
        else
        {
            //System.out.println("C��D��һ���㣬�Ҳ���AB�߶��ϣ�");
        }
        return 0;
    }

    v = ( b.y - a.y ) * ( c.x - d.x ) - ( b.x - a.x ) * ( c.y - d.y );
    if( IsDoubleEqual( v, 0 ) )
    {
        //System.out.println("�߶�ƽ�У��޽��㣡");
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

        //System.out.println("�߶��ཻ�ڵ�(" + p.x + "," + p.y + ")��");
        return 1; // '�ཻ
    }
    else
    {
        //System.out.println("�߶��ཻ���齻��(" + p.x + "," + p.y + ")��");
        return -1; // '�ཻ�������߶���
    }
}

//bool LinesegInsidePolygon(const PointArray& polygon, const DT_Point& s, const DT_Point& t)
//{
//	// �ж��߶�l�Ķ˵��Ƿ񲻶��ڶ������
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
//		// �߶�s=(si, ti)
//		DT_Point si = polygon[i];
//		DT_Point ti = polygon[(i+1)%n];
//
//		if(IsPointOnline(si, ti, s)) //�߶�l����ʼ�˵����߶�s��
//			PointSet[top++] = s;
//
//		else if(IsPointOnline(si, ti, t)) //�߶�l����ֹ�˵����߶�s��
//			PointSet[top++] = t;
//		else
//		{
//			if(IsPointOnline(s, t, si)) //�߶�s����ʼ�˵����߶�l��
//				PointSet[top++] = si;
//			else if(IsPointOnline(s, t, ti)) // �߶�s����ֹ�˵����߶�l��
//				PointSet[top++] = ti;
//			else
//			{
//				if(intersect(l,s)) // ���ʱ������ཻ���϶����ڽ�������false
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
//	for(i=0;i<top-1;i++) /* ð������x����С������ǰ�棻x������ͬ�ߣ�y����С������ǰ��*/
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
//		tmp.x=(PointSet[i].x+PointSet[i+1].x)/2; //�õ��������ڽ�����е�
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