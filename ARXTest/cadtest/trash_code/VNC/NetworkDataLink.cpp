#include "StdAfx.h"
#include "NetworkDataLink.h"

#include "config.h"

#include <vector>
typedef std::vector<double> DoubleArray;

/*
 * 放大浮点数
 * 参数：
 *		v     -- 原来的浮点数(v>0)
 *		coeff -- 放大因子(coeff>0)
 */
static double Magnify_Double( double v, double coeff )
{
    return ( 1 + v ) * coeff;
}

void FanDataLink::regDatas()
{
    linkStringData( _T( "名称" ), &name );
    linkDoubleData( _T( "风量-风压特性曲线0次项" ), &a0 );
    linkDoubleData( _T( "风量-风压特性曲线1次项" ), &a1 );
    linkDoubleData( _T( "风量-风压特性曲线2次项" ), &a2 );
    linkDoubleData( _T( "风量-风压特性曲线3次项" ), &a3 );
    linkDoubleData( _T( "风量-风压特性曲线4次项" ), &a4 );
    linkDoubleData( _T( "风量-风压特性曲线5次项" ), &a5 );
}

static void GetDoubleArray( const FanDataLink* pFanData, DoubleArray& a )
{
    a.push_back( pFanData->a0 );
    a.push_back( pFanData->a1 );
    a.push_back( pFanData->a2 );
    a.push_back( pFanData->a3 );
    a.push_back( pFanData->a4 );
    a.push_back( pFanData->a5 );
}

// m的倒n阶，及n(n-1)(n-2)...(n-m+1)
static int factorial( int m, int n )
{
    if( n > m ) return 0;

    int k = m, c = 1;
    for( int j = 0; j < n; j++ )
    {
        c *= k;
        k--;
    }
    return c;
}

// 计算多项式的导数
// 多项式f = a0 + a1*q + a2*q^2 + a3*q^3 + a4*q^4 + a5*q^5
// a  - 多项式的系数
// n  - 几阶导数
// q  - 输入的风量
static double CaclPolyomial( const DoubleArray& a, int n, double q )
{
    double s = 0, t = 1;

    int len = a.size();
    for( int i = n; i < len; i++ )
    {
        s += factorial( i, n ) * a[i] * t;
        t *= q;
    }
    return s;
}

double FanDataLink::fanPress_0( double q ) const
{
    DoubleArray a;
    GetDoubleArray( this, a );
    return CaclPolyomial( a, 0, q );
}

double FanDataLink::fanPress_1( double q ) const
{
    DoubleArray a;
    GetDoubleArray( this, a );
    return CaclPolyomial( a, 1, q );
}

EdgeDataLink::EdgeDataLink() : et( VNC_COMMON ), PFanData( 0 ), r( 0 ), q( 0 ), ar( 0 ), fq( 0 )
{

}

EdgeDataLink::~EdgeDataLink()
{
    delete PFanData;
    PFanData = 0;
}

void EdgeDataLink::regDatas()
{
    linkStringData( _T( "名称" ), &name );
    linkDoubleData( _T( "风阻" ), &r );
    linkDoubleData( _T( "调节风阻" ), &ar );
    linkDoubleData( _T( "风量" ), &q );
    linkDoubleData( _T( "固定风量" ), &fq );
}

double EdgeDataLink::edgeResistance_0() const
{
    return r * q * abs( q );
}

double EdgeDataLink::edgeResistance_1() const
{
    return abs( 2 * r * q ); // >=0
}

double EdgeDataLink::fanPressure_0() const
{
    if( PFanData == 0 ) return 0;
    return PFanData->fanPress_0( q );
}

double EdgeDataLink::fanPressure_1() const
{
    if( PFanData == 0 ) return 0;
    return PFanData->fanPress_1( q );
}

void EdgeDataLink::increase( double delta_q )
{
    q += delta_q;
}

bool EdgeDataLink::isFixedEdge() const
{
    return ( et == VNC_FIXTED );
}

bool EdgeDataLink::isValidFanEdge() const
{
    return ( PFanData != 0 );
}

bool EdgeDataLink::isVirtualAirEdge() const
{
    return ( et == VNC_AIR );
}

void EdgeDataLink::initIterateVar( bool speedUp )
{
    // 放大固定风量分支和风机分支(作为余枝)
    c = abs( speedUp ? r * q : r );

    if( PFanData != 0 )
    {
        c = Magnify_Double( c, LARGE_COEFF );  // 风机分支
    }

    if( et == VNC_FIXTED )
    {
        c = Magnify_Double( c, LARGE_COEFF ); // 固定风量分支
    }
}

void ChimneyDataLink::regDatas()
{
    linkDoubleData( _T( "风阻" ), &r );
}