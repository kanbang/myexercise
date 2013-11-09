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
private://ƽ���õ��Ĳ���
    double si0, si1, si2, si3, ssum;	//����������Բο�ƽ��ԭ����
    double ax, bx, cx, ay, by, cy;
    double si00, si10, si20, si30;		//���²���Ϊ��һ�ײ����ı���
    double ax0, bx0, cx0, ay0, by0, cy0;

    double dts;	//ƽ��ʱ���õ���̲���

private://ƽ��֮���γɵļ��ܶȵ�
    CArray <CPoint,CPoint> ptArray; //ƽ�������ܶȣ���ĵ�
    int zValue;		// ��עƽ����ĵ�ֵ��ʱʹ�õ�����

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
    void DoSmooth(CDC* dc);	//��ƽ����
    void DoLable(CDC* dc, double zValue);	//��ע
};

#endif // !defined(AFX_SMOOTH_H__586F9434_010D_4FA7_AD99_0F33188A8BBD__INCLUDED_)


