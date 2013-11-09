#include "stdafx.h"
#include "GeoAlgo.h"

#include <vector>
#include <cmath>
#include <cfloat>
#include <algorithm>

/* ��������Ͷ��� */
struct Point
{
    double x;
    double y;
};

#define MAXN 10000
Point points[MAXN];     // ��������
int S[MAXN];            // graham�㷨ʹ��
int D[MAXN * 2];        // melkman�㷨ʹ��
int n;                  // ����ĸ���

/* ��������֮����� */
double dist( Point& a, Point& b )
{
    double x = a.x - b.x;
    double y = a.y - b.y;
    return sqrt( x * x + y * y );
}

double dist_2( Point& a, Point& b )
{
    double x = a.x - b.x;
    double y = a.y - b.y;
    return ( x * x + y * y );
}

/* ��a-c�Ƿ���a-b����� */
/* a-b�Ƕ�ջS�е������㣬c������Ҫ�жϵĵ� */
int isleft( Point& a, Point& b, Point& c )
{
    return ( b.x - a.x ) * ( c.y - a.y ) - ( c.x - a.x ) * ( b.y - a.y );
}

// �������Ƚ�
int cmp_real( double x, double y )
{
    double d = x - y;
    if( d < 0 ) return 1; // d < 0
    //if(d > DBL_EPSILON) return 1; // d > 0
    return 0;   // d == 0
}

int cmp_y( int i, int j )
{
    return cmp_real( points[i].y, points[j].y );
}

// ���ռ��ǵĴ�С��������
// ����Ƕ���ͬ����ôȡ����p0��Զ�ĵ�
int cmp_angle( Point& a, Point& b )
{
    // �������0����ʾ0-b��0-a���ұ�
    // ���С��0����ʾ0-b��0-a�����
    if( isleft( points[0], a, b ) > 0 ) return 1;
    if( isleft( points[0], a, b ) < 0 ) return 0;
    if( dist( points[0], a ) < dist( points[0], b ) ) return 1;
    return 0;
}

// ������x��y����
int cmp_xy( Point& a, Point& b )
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    if( dx < 0 ) return 1;
    if( dx < DBL_EPSILON  && dy < 0 ) return 1; // dx == 0
    return 0;
}

//�ҵ�һ������,��Ϊ�㷨����ʼ����
int getStartPoint()
{
    int k = 0;
    for( int i = 0; i < n; ++i )
    {
        double xi = points[i].x;
        double yi = points[i].y;

        double xk = points[k].x;
        double yk = points[k].y;

        if( ( yi < yk ) || ( ( yi == yk ) && ( xi < xk ) ) )
        {
            k = i;
        }
    }
    return k;
}

/* ����Graham�㷨����͹������������ջS�е�λ��:[0, top] */
void graham( int& top )
{
    //�ҵ�һ������,��Ϊ�㷨����ʼ����
    int k = getStartPoint();
    Point tmp = points[0];
    points[0] = points[k];
    points[k] = tmp;

    acutPrintf( _T( "\n��1������(%.3f, %.3f)" ), points[0].x, points[0].y );

    // ��1-n-1�ĵ㰴�ռ���[0, PI]��������
    std::sort( points + 1, points + n, cmp_angle );

    for( int i = 0; i <= 2; ++i )
    {
        S[i] = i;   //��ǰ3������ѹջ
    }

    top = 2;
    for( int i = 3; i < n; i++ )
    {
        /*����µĵ�,�������ջ�е�2�㹹����һ��"��"��, ��ջ��Ԫ�س�ջ. ֱ����ջ�����*/
        while( top > 0 && isleft( points[S[top - 1]], points[S[top]], points[i] ) < 0 )
        {
            top--;
        }

        //acutPrintf(_T("top: %d"), top);
        top++;        // ���µ�ѹջ
        S[top] = i;
    }
}

/* ����melkman�㷨����͹��,������������D�е�����λ��(bot, top) */
void melkman( int& bot, int& top )
{
    // ��1-n-1�ĵ㰴��xy��������
    std::sort( points, points + n, cmp_xy );

    int i, t;

    bot = n - 1;
    top = n;
    D[top++] = 0;
    D[top++] = 1;

    for( i = 2; i < n; i++ )
    {
        //Ѱ�ҵ������� Ҫ��֤3���㲻���ߣ���
        if( isleft( points[D[top - 2]], points[D[top - 1]], points[i] ) != 0 ) break;
        D[top - 1] = i;			//���߾͸�������
    }

    D[bot--] = i;
    D[top++] = i;		//i�ǵ������� �����ߣ���

    //��ʱ��������3���㣬Ҫ��֤3����a,b,c�ǳ���ʱ��ģ����Ǿ͵���ab
    if( isleft( points[D[n]], points[D[n + 1]], points[D[n + 2]] ) < 0 )
    {
        t = D[n];
        D[n] = D[n + 1];
        D[n + 1] = t;
    }

    for( i++; i < n; i++ )
    {
        //�����������i��͹���ڣ�����
        if( isleft( points[D[top - 2]], points[D[top - 1]], points[i] ) > 0 &&
                isleft( points[D[bot + 1]], points[D[bot + 2]], points[i] ) > 0 ) continue;

        while( isleft( points[D[top - 2]], points[D[top - 1]], points[i] ) <= 0 ) top--;
        D[top++] = i;

        while( isleft( points[D[bot + 1]], points[D[bot + 2]], points[i] ) <= 0 ) bot++;
        D[bot--] = i;
    }

    // ͹��������ɣ�D������bot+1��top-1�ھ���͹��������(ͷβ��ͬһ��)
}

// �������㷨,���÷����㷨
int CPY[MAXN], CPlen; // ����Y'�Լ�����
double closestPoint_recursion( int L, int R, int& p1, int& p2 )
{
    //acutPrintf(_T("\nL=%d, R=%d"), L, R);

    if( L >= R ) return DBL_MAX;
    if( R - L == 1 )
    {
        p1 = L;
        p2 = R;
        return dist_2( points[L], points[R] );
    }

    int mid = ( L + R ) >> 1;
    int i, j;
    //for(i=mid; i>=L /*&& !cmp_real(points[i].x, points[mid].x)*/; i--);
    //acutPrintf(_T("\nmid=%d"), mid);

    int lp1, lp2;
    double d1 = closestPoint_recursion( L, mid, lp1, lp2 );
    //acutPrintf(_T("\n����������:%.15f, p1=%d, p2=%d"), d1, lp1, lp2);

    //for(i=mid; i<=R /*&& !cmp_real(points[i].x, points[mid].x)*/; i++);
    int rp1, rp2;
    double d2 = closestPoint_recursion( mid, R, rp1, rp2 );
    //acutPrintf(_T("\n�����Ҳ����:%.15f, p1=%d, p2=%d"), d2, rp1, rp2);

    double ret;
    if( ( d1 - d2 ) < 0 ) // d1 < d2
    {
        p1 = lp1;
        p2 = lp2;
        ret = d1;
    }
    else             // d1 >= d2
    {
        p1 = rp1;
        p2 = rp2;
        ret = d2;
    }

    //acutPrintf(_T("\n�Ƚ��������࣬�õ�����:%.15f, p1=%d, p2=%d"), ret, p1, p2);

    CPlen = 0;
    for( i = L; i <= R; i++ )
    {
        double dx = ( points[i].x - points[mid].x );
        //acutPrintf(_T("\n����:(%d, %d)�ĵ�%d�������м��:%d��x�����:%.15f"), L, R, i, mid, dx);
        if( ( dx * dx - ret ) < 0 )
        {
            //acutPrintf(_T("\n��%d������ӵ�Y��"), i);
            CPY[CPlen] = i;
            CPlen++;
        }
    }
    //acutPrintf(_T("\nY���鳤��:%d"), CPlen);

    std::sort( CPY, CPY + CPlen, cmp_y );

    for( i = 0; i < CPlen; i++ )
    {
        //acutPrintf(_T("\nY�е�%d��������(%.15f, %.15f)"), i, points[CPY[i]].x, points[CPY[i]].y);
        int cnt = 1;
        // ������8����cnt = [0, 7]
        for( j = i + 1; j < CPlen && cnt <= 7; j++ )
        {
            // ����Ҫ����
            double d = dist_2( points[CPY[i]], points[CPY[j]] );
            if( ( d - ret ) < 0 ) // d < ret
            {
                ret = d;
                p1 = CPY[i];
                p2 = CPY[j];
            }
            cnt++;
        }
    }
    return ret;
}

void closestPoint( AcGePoint2d& pt1, AcGePoint2d& pt2 )
{
    // Ԥ����(��x��y)
    std::sort( points, points + n, cmp_xy );

    for( int i = 0; i < n; i++ )
    {
        //acutPrintf(_T("\n����:%.15f, %.15f"), points[i].x, points[i].y);
    }
    //acutPrintf(_T("\n"));
    int p1, p2;
    double d = closestPoint_recursion( 0, n - 1, p1, p2 );

    pt1.set( points[p1].x, points[p1].y );
    pt2.set( points[p2].x, points[p2].y );
    //acutPrintf(_T("\n�������:%.15f"), d);
}

static bool ReadPoints( const AcGePoint2dArray& V )
{
    n = V.length();
    if( n > MAXN ) return false;

    for( int i = 0; i < n; i++ )
    {
        points[i].x = V[i].x;
        points[i].y = V[i].y;
    }
    return true;
}

static void Graham_GetHullPoints( int top, AcGePoint2dArray& H )
{
    for( int i = 0; i <= top; ++i )
    {
        double x = points[S[i]].x;
        double y = points[S[i]].y;
        H.append( AcGePoint2d( x, y ) );
    }
}

static void Melkman_GetHullPoints( int bot, int top, AcGePoint2dArray& H )
{
    for( int i = bot + 1; i < top - 1; i++ )
    {
        double x = points[D[i]].x;
        double y = points[D[i]].y;
        H.append( AcGePoint2d( x, y ) );
    }
}

static AcGePoint2d Point3D_To_2D( const AcGePoint3d& pt )
{
    return AcGePoint2d( pt.x, pt.y );
}

static AcGePoint3d Point2D_To_3D( const AcGePoint2d& pt )
{
    return AcGePoint3d( pt.x, pt.y, 0 );
}

bool Graham_ConvexHull_2D( const AcGePoint2dArray& V, AcGePoint2dArray& H )
{
    // ������3��������
    if( V.length() < 3 )
    {
        //acutPrintf(_T("\n��ĸ���%d < 3"), V.length());
        return false;
    }

    // ��V�ж�ȡ���ݵ�ȫ�ֱ���points��
    if( !ReadPoints( V ) )
    {
        //acutPrintf(_T("\n��ĸ���%d ���� %d"), V.length(), MAXN);
        return false;
    }

    // ʹ��GrahamScan�㷨����͹����������stack������
    // ����������������λ��top
    int top;
    graham( top );

    //acutPrintf(_T("\n���ص�S������������:%d"), top);

    // ��S�����л�ȡ͹��
    Graham_GetHullPoints( top, H );

    return true;
}

bool Melkman_ConvexHull_2D( const AcGePoint2dArray& V, AcGePoint2dArray& H )
{
    // ������3��������
    if( V.length() < 3 )
    {
        //acutPrintf(_T("\n��ĸ���%d < 3"), V.length());
        return false;
    }

    // ��V�ж�ȡ���ݵ�ȫ�ֱ���points��
    if( !ReadPoints( V ) )
    {
        //acutPrintf(_T("\n��ĸ���%d ���� %d"), V.length(), MAXN);
        return false;
    }

    // ʹ��melkman�㷨����͹����������D������
    // ��������ĵײ�bot�Ͷ���top
    int bot, top;
    melkman( bot, top );

    //acutPrintf(_T("\n���ص�D����ĵײ�:%d������:%d"), bot, top);

    // ��stack�����л�ȡ͹��
    Melkman_GetHullPoints( bot, top, H );

    return true;
}

static bool ConvexHull_3D( const AcGePoint3dArray& V, AcGePoint3dArray& H, bool useGrahamOrMelkman )
{
    AcGePoint2dArray V_2d, H_2d;
    int len = V.length();
    for( int i = 0; i < len; i++ )
    {
        V_2d.append( Point3D_To_2D( V[i] ) );
    }

    bool ret;
    if( useGrahamOrMelkman )
    {
        ret = Graham_ConvexHull_2D( V_2d, H_2d );
    }
    else
    {
        ret = Melkman_ConvexHull_2D( V_2d, H_2d );
    }

    if( ret )
    {
        H.removeAll();
        int len = H_2d.length();
        for( int i = 0; i < len; i++ )
        {
            H.append( Point2D_To_3D( H_2d[i] ) );
        }
    }
    return ret;
}

bool Graham_ConvexHull_3D( const AcGePoint3dArray& V, AcGePoint3dArray& H )
{
    return ConvexHull_3D( V, H, true );
}

bool Melkman_ConvexHull_3D( const AcGePoint3dArray& V, AcGePoint3dArray& H )
{
    return ConvexHull_3D( V, H, false );
}

bool ClosestPoint_2D( const AcGePoint2dArray& V, AcGePoint2d& pt1, AcGePoint2d& pt2 )
{
    if( !ReadPoints( V ) )
    {
        //acutPrintf(_T("\n��ĸ���%d ���� %d"), V.length(), MAXN);
        return false;
    }

    // ����������
    closestPoint( pt1, pt2 );

    return true;
}

bool ClosestPoint_3D( const AcGePoint3dArray& V, AcGePoint3d& pt1, AcGePoint3d& pt2 )
{
    AcGePoint2dArray V_2d;
    int len = V.length();
    for( int i = 0; i < len; i++ )
    {
        V_2d.append( Point3D_To_2D( V[i] ) );
    }

    AcGePoint2d pt1_2d, pt2_2d;
    bool ret = ClosestPoint_2D( V_2d, pt1_2d, pt2_2d );
    if( ret )
    {
        pt1 = Point2D_To_3D( pt1_2d );
        pt2 = Point2D_To_3D( pt2_2d );
    }
    return ret;
}

bool IsPointOnline( const AcGePoint3d& p1, const AcGePoint3d& p2, const AcGePoint3d& p )
{
    AcGeLineSeg3d line( p1, p2 );
    return ( Adesk::kTrue == line.isOn( p ) );
}

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
        if ( p.y < min( p1.y, p2.y ) ) // ������p1p2�ӳ�����
            continue;
        if ( p.y >= max( p1.y, p2.y ) ) // ������p1p2�ӳ�����
            continue;
        // �󽻵�� x ����
        double x = ( double )( p.y - p1.y ) * ( double )( p2.x - p1.x ) / ( double )( p2.y - p1.y ) + p1.x;

        if ( x > p.x )
            nCross++; // ֻͳ�Ƶ��߽���
    }
    // ���߽���Ϊż�������ڶ����֮��
    return ( nCross % 2 == 1 );
}

#define TINY_VALUE			1.5e-16

/* ������ģ��Matrix */
template<class T>
class TMatrix
{
public:
    TMatrix() : m_nWidth( 0 ), m_nHeight( 0 ), m_arByte( NULL ) {}
    TMatrix( int nWidth, int nHeight );
    TMatrix( int nWidth, int nHeight, T* pByte );
    TMatrix( const TMatrix& rhs );
    virtual ~TMatrix();
    void SetDimension( int nWidth, int nHeight );
    void ZeroAll();
    void Clean();
    TMatrix& operator=( const TMatrix& rhs );
    void operator+=( const TMatrix& rhs );
    void operator-=( const TMatrix& rhs );
    void PriorityAdd( const TMatrix& rhs );
    // inline access functions
    T& operator[]( int nIndex );
    T operator[]( int nIndex ) const;
    T& operator()( int nRow, int nCol );
    T operator()( int nRow, int nCol ) const;
    // getter
    int GetWidth() const
    {
        return m_nWidth;
    }
    int GetHeight() const
    {
        return m_nHeight;
    }
    // uitilies
    T* GetRow( int nIndex );
    T* GetArray();
    bool IsSameDimension( const TMatrix& rhs ) const;
    void MakeSameDimension( TMatrix& output ) const;

protected:
    T* m_arByte;
    int m_nWidth;
    int m_nHeight;
};

template<class T>
inline T& TMatrix<T>::operator[]( int nIndex )
{
    return m_arByte[nIndex];
}

template<class T>
inline T TMatrix<T>::operator[]( int nIndex ) const
{
    return m_arByte[nIndex];
}

template<class T>
inline T& TMatrix<T>::operator()( int nRow, int nCol )
{
    return m_arByte[nCol + nRow * m_nWidth];
}

template<class T>
inline T TMatrix<T>::operator()( int nRow, int nCol ) const
{
    return m_arByte[nCol + nRow * m_nWidth];
}

template<class T>
TMatrix<T>::TMatrix( int nWidth, int nHeight ) : m_nWidth( nWidth ), m_nHeight( nHeight )
{
    int nTotalSize = nWidth * nHeight;
    m_arByte = new T[nTotalSize];
    ::memset( m_arByte, 0, sizeof( T ) * nTotalSize );
}

template<class T>
TMatrix<T>::TMatrix( int nWidth, int nHeight, T* pByte ) : m_nWidth( nWidth ), m_nHeight( nHeight )
{
    int nTotalSize = nWidth * nHeight;
    m_arByte = new T[nTotalSize];
    ::memcpy( m_arByte, pByte, sizeof( T ) * nTotalSize );
}

template<class T>
TMatrix<T>::TMatrix( const TMatrix<T>& rhs )
{
    m_nWidth = rhs.m_nWidth;
    m_nHeight = rhs.m_nHeight;
    int nTotalSize = m_nWidth * m_nHeight;
    m_arByte = new T[nTotalSize];
    ::memcpy( m_arByte, rhs.m_arByte, sizeof( T ) * nTotalSize );
}

template<class T>
TMatrix<T>::~TMatrix()
{
    Clean();
}

template<class T>
void TMatrix<T>::SetDimension( int nWidth, int nHeight )
{
    Clean();
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    int nTotalSize = m_nWidth * m_nHeight;
    m_arByte = new T[nTotalSize];
    ::memset( m_arByte, 0, sizeof( T ) * nTotalSize );
}

template<class T>
void TMatrix<T>::ZeroAll()
{
    ::memset( m_arByte, 0, sizeof( T ) * m_nWidth * m_nHeight );
}

template<class T>
void TMatrix<T>::Clean()
{
    m_nWidth = 0;
    m_nHeight = 0;
    if( m_arByte )
    {
        delete[] m_arByte;
        m_arByte = NULL;
    }
}

template<class T>
TMatrix<T>& TMatrix<T>::operator=( const TMatrix<T>& rhs )
{
    if( &rhs == this )
        return *this;

    if( !IsSameDimension( rhs ) )
    {
        m_nWidth = rhs.m_nWidth;
        m_nHeight = rhs.m_nHeight;
        if( m_arByte )
            delete[] m_arByte;

        int nTotalSize = m_nWidth * m_nHeight;
        m_arByte = new T[nTotalSize];
    }

    ::memcpy( m_arByte, rhs.m_arByte, sizeof( T ) * m_nWidth * m_nHeight );

    return *this;
}

template<class T>
void TMatrix<T>::operator+=( const TMatrix<T>& rhs )
{
    int nTotalByte = m_nWidth * m_nHeight;
    for( int i = 0; i < nTotalByte; i++ )
    {
        m_arByte[i] += rhs[i];
        if( m_arByte[i] > 255 )
            m_arByte[i] = 255;
    }
}

template<class T>
void TMatrix<T>::operator-=( const TMatrix<T>& rhs )
{
    int nTotalByte = m_nWidth * m_nHeight;
    for( int i = 0; i < nTotalByte; i++ )
    {
        m_arByte[i] = abs( m_arByte[i] - rhs[i] );
        if( m_arByte[i] > 255 )
            m_arByte[i] = 255;
    }
}

template<class T>
void TMatrix<T>::PriorityAdd( const TMatrix<T>& rhs )
{
    int nTotalByte = m_nWidth * m_nHeight;
    for( int i = 0; i < nTotalByte; i++ )
        m_arByte[i] = max( m_arByte[i], rhs[i] );
}

template<class T>
T* TMatrix<T>::GetRow( int nIndex )
{
    T* pByte = m_arByte;
    pByte += m_nWidth * nIndex * sizeof( T );
    return pByte;
}

template<class T>
T* TMatrix<T>::GetArray() throw()
{
    return m_arByte;
}

template<class T>
bool TMatrix<T>::IsSameDimension( const TMatrix& rhs ) const throw()
{
    if( m_nWidth == rhs.m_nWidth && m_nHeight == rhs.m_nHeight )
        return true;
    else
        return false;
}

template<class T>
void TMatrix<T>::MakeSameDimension( TMatrix<T>& output ) const throw()
{
    if( output.m_nHeight != m_nHeight || output.m_nWidth != m_nWidth )
    {
        output.Clean();
        output.m_nHeight = m_nHeight;
        output.m_nWidth = m_nWidth;
        output.m_arByte = new T[m_nWidth * m_nHeight];
        ::memset( output.m_arByte, 0, sizeof( T ) * m_nHeight * m_nWidth );
    }
}

template<class T>
void LUDecompose( TMatrix<T>& A, std::vector<int>& Permutation, int& d )
{
    int n = A.GetHeight();
    std::vector<T> vv( n );
    Permutation.resize( n );

    d = 1;

    T amax;
    for( int i = 0; i < n; i++ )
    {
        amax = 0.0;
        for( int j = 0; j < n; j++ )
            if( fabs( A( i, j ) ) > amax )
                amax = fabs( A( i, j ) );

        if( amax < TINY_VALUE )
            return;

        vv[i] = 1.0 / amax;
    }

    T sum, dum;
    int imax;
    for( int j = 0; j < n; j++ )
    {
        for ( int i = 0; i < j; i++ )
        {
            sum = A( i, j );
            for( int k = 0; k < i; k++ )
                sum -= A( i, k ) * A( k, j );
            A( i, j ) = sum;
        }
        amax = 0.0;

        for( int i = j; i < n; i++ )
        {
            sum = A( i, j );
            for( int k = 0; k < j; k++ )
                sum -= A( i, k ) * A( k, j );

            A( i, j ) = sum;
            dum = vv[i] * fabs( sum );

            if( dum >= amax )
            {
                imax = i;
                amax = dum;
            }
        }

        if( j != imax )
        {
            for( int k = 0; k < n; k++ )
            {
                dum = A( imax, k );
                A( imax, k ) = A( j, k );
                A( j, k ) = dum;
            }
            d = -d;
            vv[imax] = vv[j];
        }
        Permutation[j] = imax;

        if( fabs( A( j, j ) ) < TINY_VALUE )
            A( j, j ) = TINY_VALUE;

        if( j != n )
        {
            dum = 1.0 / A( j, j );
            for( int i = j + 1; i < n; i++ )
                A( i, j ) *= dum;
        }
    }
}

template<class T>
void LUBackSub( TMatrix<T>& A, std::vector<int>& Permutation, std::vector<T>& B )
{
    int n = A.GetHeight();
    T sum;
    int ii = 0;
    int ll;
    for( int i = 0; i < n; i++ )
    {
        ll = Permutation[i];
        sum = B[ll];
        B[ll] = B[i];
        if( ii != 0 )
            for( int j = ii; j < i; j++ )
                sum -= A( i, j ) * B[j];
        else if( sum != 0.0 )\
            ii = i;
        B[i] = sum;
    }

    for( int i = n - 1; i >= 0; i-- )
    {
        sum = B[i];
        if( i < n )
        {
            for( int j = i + 1; j < n; j++ )
                sum -= A( i, j ) * B[j];
        }
        B[i] = sum / A( i, i );
    }
}

/* ������� */
template<typename T>
struct TPoint3D
{
    TPoint3D() : x( 0 ), y( 0 ), z( 0 ) {}
    TPoint3D( T xx, T yy, T zz ) : x( xx ), y( yy ), z( zz ) {}
    T x;
    T y;
    T z;
};

/* ������������� */
typedef TPoint3D<double> Point3D;
typedef std::vector<Point3D> Point3DArray;

/* ��ֵ�㷨�ӿ� */
template<class ForwardIterator>
class TInterpolater
{
public:
    virtual ~TInterpolater() {}
    virtual double GetInterpolatedZ( double xpos, double ypos, ForwardIterator first, ForwardIterator last ) = 0;
};

/* ��ֵ�㷨ģ��ʵ���� */
typedef TInterpolater<Point3DArray::iterator> Interpolater;

/* ��������֮��ľ��� */
template<class ForwardIterator>
double GetDistance( const ForwardIterator start, int i, int j )
{
    return ::sqrt( ::pow( ( ( *( start + i ) ).x - ( *( start + j ) ).x ), 2 ) + ::pow( ( ( *( start + i ) ).y - ( *( start + j ) ).y ), 2 ) );
}

template<class ForwardIterator>
double GetDistance( double xpos, double ypos, const ForwardIterator start, int i )
{
    return ::sqrt( ::pow( ( ( *( start + i ) ).x - xpos ), 2 ) + ::pow( ( ( *( start + i ) ).y - ypos ), 2 ) );
}

/* Kriging��ֵ */
template<class T, class ForwardIterator>
class TKriging : public TInterpolater<ForwardIterator>
{
public:
    TKriging( const ForwardIterator first, const ForwardIterator last, double dSemivariance ) : m_dSemivariance( dSemivariance )
    {
        m_nSize = 0;
        ForwardIterator start = first;
        while( start != last )
        {
            ++m_nSize;
            ++start;
        }

        m_matA.SetDimension( m_nSize, m_nSize );

        for( int j = 0; j < m_nSize; j++ )
        {
            for( int i = 0; i < m_nSize; i++ )
            {
                if( i == m_nSize - 1 || j == m_nSize - 1 )
                {
                    m_matA( i, j ) = 1;
                    if( i == m_nSize - 1 && j == m_nSize - 1 )
                        m_matA( i, j ) = 0;
                    continue;
                }
                m_matA( i, j ) = ::GetDistance( first, i, j ) * dSemivariance;
            }
        }
        int nD;
        LUDecompose( m_matA, m_Permutation, nD );
    }

    double GetInterpolatedZ( double xpos, double ypos, ForwardIterator first, ForwardIterator last )
    {
        std::vector<double> vecB( m_nSize );
        for( int i = 0; i < m_nSize; i++ )
        {
            double dist = ::GetDistance( xpos, ypos, first, i );
            vecB[i] = dist * m_dSemivariance;
        }
        vecB[m_nSize - 1] = 1;

        LUBackSub( m_matA, m_Permutation, vecB );

        double z = 0;
        for( int i = 0; i < m_nSize - 1; i++ )
        {
            double inputz = ( *( first + i ) ).z;
            z += vecB[i] * inputz;
        }
        //if(z < 0)
        //	z = 0;
        return z;
    }
private:
    TMatrix<T> m_matA;
    std::vector<int> m_Permutation;
    int m_nSize;
    double m_dSemivariance;
};

/* Kriging��ֵģ��ʵ����  */
typedef TKriging<double, Point3DArray::iterator> Kriging;

// �����ݴ�AcGePoint3d ת���� Point3D����
static void AcGePoint3dArray_2_PointArray( const AcGePoint3dArray& datas, Point3DArray& input )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        AcGePoint3d pt = datas[i];
        input.push_back( Point3D( pt.x, pt.y, pt.z ) );
    }
}

/* ����ʹ�õĽӿ� */
double KrigingInterpolater( const AcGePoint3dArray& datas, const AcGePoint3d& pt, double dSemivariance )
{
    Point3DArray input;
    AcGePoint3dArray_2_PointArray( datas, input );

    Interpolater* pInterpolater = new Kriging( input.begin(), input.end(), dSemivariance );
    double z = pInterpolater->GetInterpolatedZ( pt.x, pt.y, input.begin(), input.end() );
    delete pInterpolater;

    return z;
}

#include "kriging.h"
#pragma comment(lib, "mesch12b.lib")

static void KrigingInterpolater_More_1( const AcGePoint3dArray& datas, AcGePoint3dArray& pts, double dSemivariance )
{
    Point3DArray input;
    AcGePoint3dArray_2_PointArray( datas, input );

    Interpolater* pInterpolater = new Kriging( input.begin(), input.end(), dSemivariance );
    int len = pts.length();
    for( int i = 0; i < len; i++ )
    {
        double x = pts[i].x;
        double y = pts[i].y;
        double z = pInterpolater->GetInterpolatedZ( x, y, input.begin(), input.end() );
        pts[i].z = z;
    }
    delete pInterpolater;
}

typedef std::vector<alPoint3D> alPoint3DArray;

static void AcGePoint3dArray_2_alPoint3DArray( const AcGePoint3dArray& datas, alPoint3DArray& input )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        AcGePoint3d pt = datas[i];
        input.push_back( alPoint3D( pt.x, pt.y, pt.z ) );
    }
}

static void KrigingInterpolater_More_2( const AcGePoint3dArray& datas, AcGePoint3dArray& pts, double dSemivariance )
{
    alPoint3DArray input;
    AcGePoint3dArray_2_alPoint3DArray( datas, input );

    KrigingV ip( input.begin(), input.end(), 4 );
    int len = pts.length();
    for( int i = 0; i < len; i++ )
    {
        double x = pts[i].x;
        double y = pts[i].y;
        double z = ip.GetInterpolatedZ( x, y );
        pts[i].z = z;
    }
}

void KrigingInterpolater_More( const AcGePoint3dArray& datas, AcGePoint3dArray& pts, double dSemivariance )
{
    KrigingInterpolater_More_2( datas, pts, dSemivariance );
}
