// Smooth.cpp: implementation of the CSmooth class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "contour.h"
#include <cmath>
#include "Smooth.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSmooth::CSmooth()
{
    ssum = 0;
    dts = 20;
}

CSmooth::~CSmooth()
{

}


//平滑由多个点组成的折线, 并把这些平滑的线画在DC上
void CSmooth::SmoothPolyLine( const CPoint* Pts, const int Count )
{
    double x, y, s( 0 );
    int i;
    int PointsCount( Count );
    CPoint* Points = new CPoint[Count];
    ptArray.RemoveAll();

    for( i = 0; i < Count; i++ )
        Points[i] = Pts[i];
    DoPoints( Points, PointsCount ); //规范化处理，剔除重复的点

    ssum = 0;
    ptArray.Add( Points[0] );	//往数组中添加一个元素

    // 点的个数不够,退出平滑处理
    if( PointsCount < 2 )
    {
        delete []Points;
        return;
    }

    // 两个点。
    if( PointsCount == 2 )
    {
        ptArray.Add( Points[1] );
        delete []Points;
        return;
    }

    // 只有三个点
    if( PointsCount == 3 )
    {
        CalcPara( 0, true, Points );
        ax0 = ax;
        bx0 = bx;
        cx0 = cx;
        ay0 = ay;
        by0 = by;
        cy0 = cy;
        si00 = si0;
        si10 = si1;
        si20 = si2;
        si30 = si3;
        do
        {
            s += dts;
            if ( s >= si20 ) s = si20;
            x = x1( s );
            y = y1( s );
            ptArray.Add( CPoint( ( int )x, ( int )y ) );
        }
        while( s < si20 );
        delete []Points;
        return;
    }

    //有四个或更多的点
    //绘制第一个和第二个点之间的曲线
    CalcPara( 0, true, Points );
    ax0 = ax;
    bx0 = bx;
    cx0 = cx;
    ay0 = ay;
    by0 = by;
    cy0 = cy;
    si00 = si0;
    si10 = si1;
    si20 = si2;
    do
    {
        s += dts;
        if ( s >= si10 ) s = si10;
        x = x1( s );
        y = y1( s );
        ptArray.Add( CPoint( ( int )x, ( int )y ) );
    }
    while( s < si10 );
    //绘制中间点的曲线
    for ( i = 0; i < PointsCount - 3; i++ )
    {
        CalcPara( i, true, Points );
        ax0 = ax;
        bx0 = bx;
        cx0 = cx;
        ay0 = ay;
        by0 = by;
        cy0 = cy;
        si00 = si0;
        si10 = si1;
        si20 = si2;
        si30 = si3;
        ssum = si10;
        CalcPara( i + 1, false, Points );

        s = si10;
        do
        {
            s += dts;
            if ( s >= si20 ) s = si20;
            x = a( s ) * x1( s ) + ( 1 - a( s ) ) * x2( s );
            y = a( s ) * y1( s ) + ( 1 - a( s ) ) * y2( s );
            ptArray.Add( CPoint( ( int )x, ( int )y ) );
        }
        while( s < si20 );
    }
    //绘制最后的点与倒数第二个点之间的曲线
    s = si20;
    do
    {
        s += dts;
        if ( s >= si2 ) s = si2;
        x = x2( s );
        y = y2( s );
        ptArray.Add( CPoint( ( int )x, ( int )y ) );
    }
    while( s < si2 );
    delete []Points;
}

double CSmooth::x1( const double s )
{
    return ( ax0 + bx0 + cx0 ) * s * s - ( ax0 * ( si10 + si20 ) + bx0 * ( si00 + si20 ) + cx0 * ( si00 + si10 ) ) * s + ( ax0 * si10 * si20 + bx0 * si20 * si00 + cx0 * si00 * si10 );
}

double CSmooth::y1( const double s )
{
    return ( ay0 + by0 + cy0 ) * s * s - ( ay0 * ( si10 + si20 ) + by0 * ( si00 + si20 ) + cy0 * ( si00 + si10 ) ) * s + ( ay0 * si10 * si20 + by0 * si20 * si00 + cy0 * si00 * si10 );
}

double CSmooth::x2( const double s )
{
    return ( ax + bx + cx ) * s * s - ( ax * ( si1 + si2 ) + bx * ( si0 + si2 ) + cx * ( si0 + si1 ) ) * s + ( ax * si1 * si2 + bx * si2 * si0 + cx * si0 * si1 );
}

double CSmooth::y2( const double s )
{
    return ( ay + by + cy ) * s * s - ( ay * ( si1 + si2 ) + by * ( si0 + si2 ) + cy * ( si0 + si1 ) ) * s + ( ay * si1 * si2 + by * si2 * si0 + cy * si0 * si1 );
}

double CSmooth::a( const double s )
{
    return ( si20 - s ) / ( si20 - si10 );
}

//算法中的处理
void CSmooth::CalcPara( const int i, bool bCalcS, const CPoint* Points )
{
    si0 = ssum;
    double x1 = ( Points[i + 1].x - Points[i].x ) * ( Points[i + 1].x - Points[i].x ) + ( Points[i + 1].y - Points[i].y ) * ( Points[i + 1].y - Points[i].y );
    si1 = si0 + sqrt( x1 );
    double x2 = ( Points[i + 2].x - Points[i + 1].x ) * ( Points[i + 2].x - Points[i + 1].x ) + ( Points[i + 2].y - Points[i + 1].y ) * ( Points[i + 2].y - Points[i + 1].y );
    si2 = si1 + sqrt( x2 );
//	si3=si2+sqrt((Points[i+2].x-Points[i+3].x)*(Points[i+2].x-Points[i+3].x)+(Points[i+2].y-Points[i+3].y)*(Points[i+2].y-Points[i+3].y));
    ax = Points[i].x / ( ( si0 - si1 ) * ( si0 - si2 ) );
    bx = Points[i + 1].x / ( ( si1 - si0 ) * ( si1 - si2 ) );
    cx = Points[i + 2].x / ( ( si2 - si0 ) * ( si2 - si1 ) );
    ay = Points[i].y / ( ( si0 - si1 ) * ( si0 - si2 ) );
    by = Points[i + 1].y / ( ( si1 - si0 ) * ( si1 - si2 ) );
    cy = Points[i + 2].y / ( ( si2 - si0 ) * ( si2 - si1 ) );
}

//消除重复的点
void CSmooth::DoPoints( CPoint* Points, int& PointsCount )
{
    for ( int i = 0 ; i < PointsCount - 1; i++ )
    {
        if ( Points[i] == Points[i + 1] )
        {
            for ( int j = i; j < PointsCount - 1; j++ )
                Points[j] = Points[j + 1];
            PointsCount--;
            i--;
        }
    }
}


//画平滑线
void CSmooth::DoSmooth( CDC* dc )
{
    CPoint point;
    point = ptArray.ElementAt( 0 );
    dc->MoveTo( point );
    for ( int i = 1; i < ptArray.GetSize(); i++ )
    {
        point = ptArray.ElementAt( i );
        dc->LineTo( point );
    }
}

//标注
void CSmooth::DoLable( CDC* dc, double zvalue )
{
    //	dc->Ellipse(ptArray.GetAt(0).x-50,ptArray.GetAt(0).y-50,ptArray.GetAt(0).x+50,ptArray.GetAt(0).y+50);
    //if (zvalue!=1750) return;
    int i;
    int kkkmmm = 0;
    double bzjd_m;
    double dX1, dY1, dX2, dY2, dX3, dY3;
    const double pie = 3.1416;
    CString TextString;
    LOGFONT myFont;
    double r1;
    bool bSearched;

    myFont.lfHeight = 780;
    myFont.lfWidth = 460;
    myFont.lfEscapement = 0;
    myFont.lfWeight = 400;
    myFont.lfItalic = 0;
    myFont.lfUnderline = 0;
    myFont.lfStrikeOut = 0;
    myFont.lfOutPrecision = 0;
    myFont.lfClipPrecision = 0;
    myFont.lfQuality = 0;
    myFont.lfPitchAndFamily = 0;
    myFont.lfCharSet = 0;
    strcpy( myFont.lfFaceName, "Arial" );

    //寻找标注点的位置及字体的标注角度
    r1 = 0;
    int nnk = 0;
    double RS = 0;
    i = ptArray.GetSize() - 1;
    TextString.Format( "%7.2f", zvalue );

    int iWidth = TextString.GetLength() * myFont.lfWidth;
    int iHeight = myFont.lfHeight;
    int iNeedDots = ( int )floor( iWidth / dts ) + 1;

    if ( i >= 10 )
    {
        for ( int t = 5; t <= i - 10; t++ )
        {
            bSearched = false;//假设没有找到。。。
            int iik = t;
            dX1 = ptArray.GetAt( iik ).x;
            dY1 = ptArray.GetAt( iik ).y;
            dX2 = ptArray.GetAt( iik + 1 ).x;
            dY2 = ptArray.GetAt( iik + 1 ).y;
            dX3 = ptArray.GetAt( iik + 2 ).x;
            dY3 = ptArray.GetAt( iik + 2 ).y;
            double dist = sqrt( ( dX1 - dX2 ) * ( dX1 - dX2 ) + ( dY1 - dY2 ) * ( dY1 - dY2 ) ) + sqrt( ( dX3 - dX2 ) * ( dX3 - dX2 ) + ( dY3 - dY2 ) * ( dY3 - dY2 ) );
            double mmk, kkm, xc, yc, ro;
            do
            {
                double pf1 = dX1 * dX1 + dY1 * dY1 - dX2 * dX2 - dY2 * dY2;
                double pf2 = dX2 * dX2 + dY2 * dY2 - dX3 * dX3 - dY3 * dY3;
                if ( dY1 == dY2 ) goto lbl100;
                mmk = pf2 - ( ( dY2 - dY3 ) / ( dY1 - dY2 ) ) * pf1;
                kkm = 2 * ( dX2 - dX3 - ( dY2 - dY3 ) * ( dX1 - dX2 ) / ( dY1 - dY2 ) );
                if ( abs( kkm ) < 0.0001 ) goto lbl100;
                xc = mmk / kkm;//（xc,yc）为园心，ro为半径
                yc = ( pf2 - 2 * ( dX1 - dX2 ) * xc ) / ( 2 * ( dY1 - dY2 ) );
                ro = sqrt( ( yc - dX2 ) * ( yc - dX2 ) + ( yc - dY2 ) * ( yc - dY2 ) );
                if ( ro < myFont.lfHeight * 5 )
                {
                    bSearched = false;
                    break;
                }

lbl100:		  //如果满足要求的话，继续。。。
                bSearched = true;
                if ( dist > iWidth ) break;
                iik++;
                if ( iik + 2 > i - 10 )
                {
                    bSearched = false;  //点的个数不够，停止追踪
                    break;
                }
                dX1 = dX2;
                dY1 = dY2;
                dX2 = dX3;
                dY2 = dY3;
                dX3 = ptArray.GetAt( iik + 2 ).x;
                dY3 = ptArray.GetAt( iik + 2 ).y;
                dist += sqrt( ( dX3 - dX2 ) * ( dX3 - dX2 ) + ( dY3 - dY2 ) * ( dY3 - dY2 ) );
//		dc->Ellipse(ptArray.GetAt(iik+2).x-10,ptArray.GetAt(iik+2).y-10,ptArray.GetAt(iik+2).x+10,ptArray.GetAt(iik+2).y+10);
            }
            while( 1 );
            if ( bSearched == true )
            {
                kkkmmm = t;
                break;
            }
        }
        if ( bSearched == false || kkkmmm == 0 )
        {
            return;
        }
        dc->Ellipse( ptArray.GetAt( kkkmmm ).x - 40, ptArray.GetAt( kkkmmm ).y - 40, ptArray.GetAt( kkkmmm ).x + 40, ptArray.GetAt( kkkmmm ).y + 40 );

        double xzwz = ptArray.GetAt( kkkmmm ).x;
        double yzwz = ptArray.GetAt( kkkmmm ).y;

        double dltx = ptArray.GetAt( kkkmmm ).x - ptArray.GetAt( kkkmmm + iNeedDots ).x;
        double dlty = ptArray.GetAt( kkkmmm ).y - ptArray.GetAt( kkkmmm + iNeedDots ).y;
        double fzqz;

        if ( dltx != 0 )
        {
            fzqz = -atan( dlty / dltx );
            if ( dlty == 0 ) fzqz = 0;
        }
        else
            double fzqz = 3 * pie / 2;

        bzjd_m = -fzqz;
        if ( ( fzqz < pie / 2 && fzqz > 0 ) || ( fzqz < -3 * pie / 2 && fzqz > -2 * pie ) || ( fzqz > pie && fzqz < 3 * pie / 2 ) || ( fzqz > -pie && fzqz < -pie / 2 ) )
        {
//			xzwz += (sqrt(iHeight*iHeight+iWidth*iWidth)/2 * cos(fzqz - atan(iHeight/iWidth)));
//			yzwz += (sqrt(iHeight*iHeight+iWidth*iWidth)/2 * sin(fzqz - atan(iHeight/iWidth)));
            //xzwz = xzwz - abs(myFont.lfWidth * cos(fzqz)) - abs(myFont.lfHeight * sin(fzqz) / 2);
//			yzwz = yzwz - abs(myFont.lfHeight * cos(fzqz) / 2) + abs(myFont.lfWidth * sin(fzqz));
        }
        else
        {
//			xzwz -= (sqrt(iHeight*iHeight+iWidth*iWidth)/2 * cos(fzqz - atan(iHeight/iWidth)));
//			yzwz += (sqrt(iHeight*iHeight+iWidth*iWidth)/2 * sin(fzqz - atan(iHeight/iWidth)));
//			xzwz = xzwz - abs(myFont.lfWidth * cos(fzqz)) + abs(myFont.lfHeight * sin(fzqz) / 2);
//			yzwz = yzwz - abs(myFont.lfHeight * cos(fzqz) / 2) - abs(myFont.lfWidth * sin(fzqz));
        }

        myFont.lfEscapement = fzqz * 1800 / pie;
        //标注
        CFont NewFont;
        NewFont.CreateFontIndirect( &myFont );
        CFont* OrgFont = dc->SelectObject( &NewFont );
        dc->TextOut( xzwz, yzwz, TextString, 5 ); //TextString.GetLength());
        dc->SelectObject( OrgFont );
        NewFont.DeleteObject();
        OrgFont->DeleteObject();
    }
}