// SomeFuncs.h: interface for the CSomeFuncs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOMEFUNCS_H__6910AA9E_91C6_4D9C_917D_609B3B1602B7__INCLUDED_)
#define AFX_SOMEFUNCS_H__6910AA9E_91C6_4D9C_917D_609B3B1602B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include "structdef.h"

//两条线段的关系
enum enumRelationOfLines
{
    ROL_PARA_DISTANCE	= 0x01,			//平行, 线段距离大于0
    ROL_PARA_OVERLAP	= 0x02,			//平行,线段距离为0,部分或全部重叠
    ROL_PARA_NOOVERNODISTANCE = 0x04,	//平行，线段距离为0，同时不重叠
    ROL_PARA_LINK	=0x100,				//平行，线段距离为0，两条线段相连。
    ROL_NOPARA_INTERCROSS	= 0x08,		//不平行，且内交
    ROL_NOPARA_FIRSTLINE	= 0x10,		//不平行，交点在的一条线段上，但不再第二条线段上
    ROL_NOPARA_SECONDLINE	= 0x20,		//不平行，交点在第二条线段上， 但不在的一条线段上
    ROL_NOPARA_BOTHLINES	= 0x40,		//不平行，交点在两条线段的端点上
    ROL_NOPARA_NOCROSS		= 0x80,		//不平行，不相交
};

class CSomeFuncs //: public CObject
{
public:
    CSomeFuncs();
    virtual ~CSomeFuncs();

public:
    //判断线与线之间的关系
    UINT Relation_Of_Lines(CPoint pt0, CPoint pt1, CPoint pt2, CPoint pt3, UINT UINTRelation);
    UINT Relation_Of_Lines(int x0, int y0, int x1, int y1,  int x2, int y2, int x3, int y3, UINT UINTRelation);

    //高斯算法和曲面方程
    double tend(SPoint* srcPnt, SPoint& desPnt);
    void Gauss(double a[][7], double b[7], int n, int& status); //n为数组a,b的实际大小

    //搜寻最大最小值
    void SearchMaxMin(double src[], int number, double& maximum, double& minimum, bool bHasInited = false);
    void SearchMaxMin(CArray<double, double>& src, double& maximum, double& minimum, bool bHasInited = false);

};

#endif // !defined(AFX_SOMEFUNCS_H__6910AA9E_91C6_4D9C_917D_609B3B1602B7__INCLUDED_)
