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

//�����߶εĹ�ϵ
enum enumRelationOfLines
{
    ROL_PARA_DISTANCE	= 0x01,			//ƽ��, �߶ξ������0
    ROL_PARA_OVERLAP	= 0x02,			//ƽ��,�߶ξ���Ϊ0,���ֻ�ȫ���ص�
    ROL_PARA_NOOVERNODISTANCE = 0x04,	//ƽ�У��߶ξ���Ϊ0��ͬʱ���ص�
    ROL_PARA_LINK	=0x100,				//ƽ�У��߶ξ���Ϊ0�������߶�������
    ROL_NOPARA_INTERCROSS	= 0x08,		//��ƽ�У����ڽ�
    ROL_NOPARA_FIRSTLINE	= 0x10,		//��ƽ�У������ڵ�һ���߶��ϣ������ٵڶ����߶���
    ROL_NOPARA_SECONDLINE	= 0x20,		//��ƽ�У������ڵڶ����߶��ϣ� �����ڵ�һ���߶���
    ROL_NOPARA_BOTHLINES	= 0x40,		//��ƽ�У������������߶εĶ˵���
    ROL_NOPARA_NOCROSS		= 0x80,		//��ƽ�У����ཻ
};

class CSomeFuncs //: public CObject
{
public:
    CSomeFuncs();
    virtual ~CSomeFuncs();

public:
    //�ж�������֮��Ĺ�ϵ
    UINT Relation_Of_Lines(CPoint pt0, CPoint pt1, CPoint pt2, CPoint pt3, UINT UINTRelation);
    UINT Relation_Of_Lines(int x0, int y0, int x1, int y1,  int x2, int y2, int x3, int y3, UINT UINTRelation);

    //��˹�㷨�����淽��
    double tend(SPoint* srcPnt, SPoint& desPnt);
    void Gauss(double a[][7], double b[7], int n, int& status); //nΪ����a,b��ʵ�ʴ�С

    //��Ѱ�����Сֵ
    void SearchMaxMin(double src[], int number, double& maximum, double& minimum, bool bHasInited = false);
    void SearchMaxMin(CArray<double, double>& src, double& maximum, double& minimum, bool bHasInited = false);

};

#endif // !defined(AFX_SOMEFUNCS_H__6910AA9E_91C6_4D9C_917D_609B3B1602B7__INCLUDED_)
