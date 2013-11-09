#include "StdAfx.h"
#include "NetworkDataLink.h"

#include "config.h"

#include <vector>
typedef std::vector<double> DoubleArray;

/*
 * �Ŵ󸡵���
 * ������
 *		v     -- ԭ���ĸ�����(v>0)
 *		coeff -- �Ŵ�����(coeff>0)
 */
static double Magnify_Double( double v, double coeff )
{
    return ( 1 + v ) * coeff;
}

void FanDataLink::regDatas()
{
    linkStringData( _T( "����" ), &name );
    linkDoubleData( _T( "����-��ѹ��������0����" ), &a0 );
    linkDoubleData( _T( "����-��ѹ��������1����" ), &a1 );
    linkDoubleData( _T( "����-��ѹ��������2����" ), &a2 );
    linkDoubleData( _T( "����-��ѹ��������3����" ), &a3 );
    linkDoubleData( _T( "����-��ѹ��������4����" ), &a4 );
    linkDoubleData( _T( "����-��ѹ��������5����" ), &a5 );
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

// m�ĵ�n�ף���n(n-1)(n-2)...(n-m+1)
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

// �������ʽ�ĵ���
// ����ʽf = a0 + a1*q + a2*q^2 + a3*q^3 + a4*q^4 + a5*q^5
// a  - ����ʽ��ϵ��
// n  - ���׵���
// q  - ����ķ���
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
    linkStringData( _T( "����" ), &name );
    linkDoubleData( _T( "����" ), &r );
    linkDoubleData( _T( "���ڷ���" ), &ar );
    linkDoubleData( _T( "����" ), &q );
    linkDoubleData( _T( "�̶�����" ), &fq );
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
    // �Ŵ�̶�������֧�ͷ����֧(��Ϊ��֦)
    c = abs( speedUp ? r * q : r );

    if( PFanData != 0 )
    {
        c = Magnify_Double( c, LARGE_COEFF );  // �����֧
    }

    if( et == VNC_FIXTED )
    {
        c = Magnify_Double( c, LARGE_COEFF ); // �̶�������֧
    }
}

void ChimneyDataLink::regDatas()
{
    linkDoubleData( _T( "����" ), &r );
}