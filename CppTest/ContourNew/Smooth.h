// Smooth.h: interface for the CSmooth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMOOTH_H__586F9434_010D_4FA7_AD99_0F33188A8BBD__INCLUDED_)
#define AFX_SMOOTH_H__586F9434_010D_4FA7_AD99_0F33188A8BBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CSmooth
{
private://平滑用到的参数
    double si0, si1, si2, si3, ssum;	//参数意义可以参考平滑原理部分
    double ax, bx, cx, ay, by, cy;
    double si00, si10, si20, si30;		//余下参数为第一套参数的备份
    double ax0, bx0, cx0, ay0, by0, cy0;

    double dts;	//平滑时采用的最短步长

private://平滑之后形成的加密度点
    CArray <CPoint,CPoint> ptArray; //平滑（加密度）后的点
    int zValue;		// 标注平滑后的等值线时使用的数据

public:
    CSmooth();
    virtual ~CSmooth();

private:
    void CalcPara(const int i, bool bCalcS, const CPoint* Points);
    double x1(const double s);
    double y1(const double s);
    double x2(const double s);
    double y2(const double s);
    double a(const double s);
    void DoPoints(CPoint* points, int& PointsCount);

public:
    void SmoothPolyLine(const CPoint* Points, const int PointsCount);
    void DoSmooth(CDC* dc);	//画平滑线
    void DoLable(CDC* dc, double zValue);	//标注
};

#endif // !defined(AFX_SMOOTH_H__586F9434_010D_4FA7_AD99_0F33188A8BBD__INCLUDED_)


