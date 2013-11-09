#include "stdafx.h"

#include <vector>
#include <cmath>

#define TINY_VALUE			1.5e-16

/* 矩阵类模板Matrix */
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

/* 坐标点类 */
template<typename T>
struct TPoint3D
{
    TPoint3D() : x( 0 ), y( 0 ), z( 0 ) {}
    TPoint3D( T xx, T yy, T zz ) : x( xx ), y( yy ), z( zz ) {}
    T x;
    T y;
    T z;
};

/* 定义坐标点数组 */
typedef TPoint3D<double> Point3D;
typedef std::vector<Point3D> Point3DArray;

/* 插值算法接口 */
template<class ForwardIterator>
class TInterpolater
{
public:
    virtual ~TInterpolater() {}
    virtual double GetInterpolatedZ( double xpos, double ypos, ForwardIterator first, ForwardIterator last ) = 0;
};

/* 插值算法模板实例化 */
typedef TInterpolater<Point3DArray::iterator> Interpolater;

/* 计算两点之间的距离 */
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

/* Kriging插值 */
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
        if( z < 0 )
            z = 0;
        return z;
    }
private:
    TMatrix<T> m_matA;
    std::vector<int> m_Permutation;
    int m_nSize;
    double m_dSemivariance;
};

/* Kriging插值模板实例化  */
typedef TKriging<double, Point3DArray::iterator> Kriging;

// 将数据从AcGePoint3d 转换成 Point3D类型
static void AcGePoint3dArray_2_PointArray( const AcGePoint3dArray& datas, Point3DArray& input )
{
    int len = datas.length();
    for( int i = 0; i < len; i++ )
    {
        AcGePoint3d pt = datas[i];
        input.push_back( Point3D( pt.x, pt.y, pt.z ) );
    }
}

/* 对外使用的接口 */
double KrigingInterpolater( const AcGePoint3dArray& datas, const AcGePoint3d& pt, double dSemivariance )
{
    Point3DArray input;
    AcGePoint3dArray_2_PointArray( datas, input );

    Interpolater* pInterpolater = new Kriging( input.begin(), input.end(), dSemivariance );
    double z = pInterpolater->GetInterpolatedZ( pt.x, pt.y, input.begin(), input.end() );
    delete pInterpolater;

    return z;
}

void KrigingInterpolater_More( const AcGePoint3dArray& datas, AcGePoint3dArray& pts, int nDiameter, double dSemivariance )
{
    Point3DArray input;
    AcGePoint3dArray_2_PointArray( datas, input );

    Interpolater* pInterpolater = new Kriging( input.begin(), input.end(), dSemivariance );
    int len = pts.length();
    for( int i = 0; i < len; i++ )
    {
        double xpos = pts[i].x;
        double ypos = pts[i].y;
        double z = pInterpolater->GetInterpolatedZ( xpos, ypos, input.begin(), input.end() );
        pts[i].z = z;
    }
    delete pInterpolater;
}
