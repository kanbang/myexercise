// BiaoZhu.cpp: implementation of the CBiaoZhu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "contour.h"
#include "BiaoZhu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBiaoZhu::CBiaoZhu()
{

}

CBiaoZhu::~CBiaoZhu()
{

}

/*void CBiaoZhu::biaozhu(int* id, int* idx, int dzxm, double xa, double xi, double ya, double yi, CDC* dc)
{
	int i, j;
	int  a, b, c, k;
	const double pie=3.14159;
	int xx, yy;
	int kkkmmm, kkkmmm1, kkkmmm2;
	double bzjd_m;
	double x1,y1;
	double xiabian, xbian, xsjj1, yzjj1;
	int ja;
	LOGFONT myFont; //定义逻辑字体
	CFont NewFont, *pOrgFont;

	obj.ScaleMode = 1
	ax = obj.ScaleWidth
	ay = obj.ScaleHeight
	obj.ScaleMode = 0
	dx = obj.ScaleWidth
	dy = obj.ScaleHeight

	xby = dx / dy;	//宽度和高度之比
	dmx = xa - xi;	//xa和ya可能为左上角坐标,二xi,yi可能为右下角坐标
	dmy = ya - yi;
	mxby = dmx / dmy;

	if( mxby >= xby )
	{
		yi = yi - (yi + dmx / xby - ya) / 2;
		ya = ya + (yi + dmx / xby - ya) / 2;
	}
	else
	{
		xi = xi - (xi + xby * dmy - xa) / 2;
		xa = xa + (xi + xby * dmy - xa) / 2;
	}

	obj.Scale (xi, ya)-(xa, yi)
	bx = obj.ScaleWidth
	by = obj.ScaleHeight

	fillba = bx / ax;
	fillbb = -by / ay;

	if( x[1] == 0 ) return;

    int throw2;
    myFont.lfHeight = 13;
    myFont.lfWidth = 6;
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
    myFont.lfFaceName = "Arial" + Chr$(0);

    obj.FontSize = 8;
    obj.ForeColor = RGB(0, 0, 0);

	r1 = 0;
	k = 0;
	nnk = 0;
	RS = 0;
	dzxds = idx[dzxm, 1];
	For ja = 0 To dzxm;
	  if( ja = 0 )
		i1 = 0;
	  else
		i1 = idx[ja - 1][1] + 1;
	  i2 = idx[ja, 1];
	  k = i2 - i1;

     i = i2 + 1;
     TextString = z(i - Int(k / 2));
     if( k >= 10 )
	 {
       if( i - k + 5 < i - 5 )
	   {
        for (iik = i - k + 5; iik<=i - 5; iik++)
		{
          x1 = x[iik - 1];
          y1 = y[iik - 1];
          x2 = x[iik];
          y2 = y[iik];
          x3 = x[iik + 1];
          y3 = y[iik + 1];
          pf1 = x1 ^ 2 + y1 ^ 2 - x2 ^ 2 - y2 ^ 2;
          pf2 = x2 ^ 2 + y2 ^ 2 - x3 ^ 2 - y3 ^ 2;
          if( y1 == y2 ) {kkkmmm = iik;break; }
          mmk = pf2 - ((y2 - y3) / (y1 - y2)) * pf1;
          kkm = 2 * (x2 - x3 - (y2 - y3) * (x1 - x2) / (y1 - y2));
          if( Abs(kkm) < 0.0001 ) {kkkmmm = iik;break;}
          xc = mmk / kkm;
          yc = (pf2 - 2 * (x1 - x2) * xc) / (2 * (y1 - y2));
          ro = (xc - x2) ^ 2 + (y[c - y2) ^ 2;
          ro = sqrt(ro);
          RS = RS + ro;
          nnk = nnk + 1;
          if( nnk >= 5 )
		  {
			rr = RS / 5;
            if( r1 < rr ) {r1 = rr;kkkmmm = iik - 3;RS = RS - ro;}
          }
        }

       if( kkkmmm == 0 )  GoTo 1000;

        xzwz = x[kkkmmm] - xi;
        yzwz = ya - y[kkkmmm];

        xzwz = xzwz / fillb / fillba;
        yzwz = yzwz / fillb / fillbb;
        dltx = x[kkkmmm + 1] - x[kkkmmm - 1];
        dlty = y[kkkmmm + 1] - y[kkkmmm - 1];

            if( dltx != 0 )
			{
             zqz = dlty / dltx;
             fzqz = Atn(zqz);
             if( dlty == 0 )  fzqz = 0;
            }
			else
                 fzqz = 3 * pie / 2;

            bzjd_m = fzqz;
            if( (fzqz < pie / 2 && fzqz > 0) || (fzqz > pie && fzqz < 3 * pie / 2) || (fzqz > -pie && fzqz < -pie / 2) )
			{
               xzwz = xzwz - Abs(15 * Cos(fzqz)) - Abs(13 * Sin(fzqz) / 2);
               yzwz = yzwz - Abs(13 * Cos(fzqz) / 2) + Abs(15 * Sin(fzqz));
            }
			else
			{
               xzwz = xzwz - Abs(15 * Cos(fzqz)) + Abs(13 * Sin(fzqz) / 2);
               yzwz = yzwz - Abs(13 * Cos(fzqz) / 2) - Abs(15 * Sin(fzqz));
            }

            fzqz = fzqz * 1800 / pie;

            myFont.lfEscapement = fzqz;
            xx = xzwz;
            yy = yzwz;

            NewFont.CreateFontIndirect(myFont);
            pOrgFont=dc->SelectObject(&NewFont);
            throw2 = dc->TextOut(xx, yy, TextString, Len(TextString));
            NewFont = SelectObject(obj.hdc, pOrgFont);
            throw2 = NewFont.DeleteObject();
      }
        r1 = 0;
     }
1000:
	if( kkkmmm <> 0 )
	{
		if( x[kkkmmm) <= x[kkkmmm + 1) )
		{
			for( j = kkkmmm; j<= i - 1; j++)
			{
				ddd1 = Sqr((x[j) - x[kkkmmm)) ^ 2 + (y[(j) - y[(kkkmmm)) ^ 2);
				if( ddd1 >= obj.TextWidth(Trim(TextString)) / 2 ) { kkkmmm2 = j;break;}
			}
			for(j = kkkmmm; j>=(i - k + 1); j--)
			{
				ddd1 = sqrt((x[j) - x[kkkmmm)) ^ 2 + (y[(j) - y[(kkkmmm)) ^ 2);
				if( ddd1 >= obj.TextWidth(Trim(TextString)) / 2 ) {kkkmmm1 = j;break;}
			}
		}
		else
		{
			for j = kkkmmm; j<=i - 1; j++)
			{
				ddd1 = Sqr((x[j) - x[kkkmmm)) ^ 2 + (y[(j) - y[(kkkmmm)) ^ 2);
				if( ddd1 >= obj.TextWidth(Trim(TextString)) / 2 ) {kkkmmm1 = j; break;}
			}
			for(j = kkkmmm; j>=i - k; j--)
			{
				ddd1 = Sqr((x[j) - x[kkkmmm)) ^ 2 + (y[(j) - y[(kkkmmm)) ^ 2);
				if( ddd1 >= obj.TextWidth(Trim(TextString)) / 2 ) {kkkmmm2 = j; break;}
			}
		}

        Func2ForBiaoZhu(i, k, kkkmmm, kkkmmm1, kkkmmm2, x1, y1, dc);
     }
     k = 0
Next ja
sdkg = 1
}



// BiaoZhu中间函数
void CBiaoZhu::Func1ForBiaoZhu(const int kkkmmm, double& x1, double& y1, CDC *dc, int signofx, int signofy )
{//signofx=1 or -1; signofy=1 or -1
	double xiabian xbian, xsjj1, ysjj1;

	xiabian = sqrt((x[kkkmmm + 1] - x[kkkmmm - 1]) ^ 2 + (y[kkkmmm + 1] - y[kkkmmm - 1]) ^ 2)
	xbian = 7 * obj.TextWidth(Trim(TextString)) / 12
	xsjjl = x[kkkmmm + 1] - x[kkkmmm - 1]
	ysjjl = y[kkkmmm + 1] - y[kkkmmm - 1]
	x1 = x[kkkmmm] - signofx*Abs(xbian * xsjjl / xiabian)
	if( ysjjl > 0 )
		y1 = y[kkkmmm] - signofy*Abs(xbian * ysjjl / xiabian)
	else
		y1 = y[kkkmmm] + signofy*Abs(xbian * ysjjl / xiabian)
}

// BiaoZhu中间函数
void CBiaoZhu::Func2ForBiaoZhu(const int i, const int k, const int kkkmmm, const int kkkmmm1, const int kkkmmm2, double& x1, double& y1, CDC *dc)
{
	int km1, km2;
	int j, signofx, signofy;

	if( kkkmmm1 <= kkkmmm2 )
	{ km1 = kkkmmm1; km2 = kkkmmm2;signofx = 1; signofy = 1;}
	else
	{ km1 = kkkmmm2; km2 = kkkmmm1;signofx = -1; signofy = 1;}

	for( j = i - k - 1; j<=km1 - 1;j++)
	{
		if( j = km1 - 1 )
		{
			Func1ForBiaoZhu(kkkmmm, x1, y1, dc, signofx, signofy);
			obj.Line (x[j), y[(j))-(x1, y1), RGB(0, 0, 0)
		}
		else
			obj.Line (x[j), y[(j))-(x[j + 1), y[(j + 1)), RGB(0, 0, 0)
	}
	for(j = km2; j<=i - 2; j++)
	{
		if( j > 0 && id(j) == id(j + 1))
		{
				if( j == km2 )
				{
					Func1ForBiaoZhu(kkkmmm, x1, y1, dc, (-1)*signofx, (-1)*signofy);
					obj.Line (x1, y1)-(x[j + 1), y[(j + 1)), RGB(0, 0, 0)
				}
				else
					obj.Line (x[j), y[(j))-(x[j + 1), y[(j + 1)), RGB(0, 0, 0)

			}
	}
}*/