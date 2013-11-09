#pragma once

#include "GeoDef.h"

// �жϸ������Ƿ����
extern bool IsDoubleEqual(double a, double b);

// �ж��Ƿ��е�ֵ��
// 1 -- ��  -1) û�� 0 -- ��z1��z2���
extern int InRange(double z1, double z2, double z);

// ���Բ�ֵ
extern void LinearInterpolate(const DT_Point& p1, const DT_Point& p2, DT_Point& p);

// �ж������Ƿ���ͬ(������zֵ)
extern bool IsPointEqual(const DT_Point& p1, const DT_Point& p2);

// �жϵ��Ƿ��ڶ������
extern bool IsPointInPolygon (const DT_Point& p, const PointArray& ptPolygon) ;

// �жϵ��Ƿ����߶���
extern bool IsPointOnline(const DT_Point& p1, const DT_Point& p2, const DT_Point& p);

// ����֮����е�
extern DT_Point MidPoint(const DT_Point& p1, const DT_Point& p2);

// ���㹹�ɵ�ֱ�߽Ƕȷ���
extern double Direction(const DT_Point& p1, const DT_Point& p2);

// �������
extern double Distance(const DT_Point& p1, const DT_Point& p2);

// ��������ƽ��
extern double Distance_2(const DT_Point& p1, const DT_Point& p2);

// �����p��ֱ��(p1, p2)�ľ���
extern double GetPointDistanceToLine(const DT_Point& p, const DT_Point& p1, const DT_Point& p2);

// �����������
extern double PolygonArea(const PointArray& polygon);

// ������������
// http://local.wasp.uwa.edu.au/~pbourke/geometry/polyarea/
// http://en.wikipedia.org/wiki/Centroid#Centroid_of_polygon
extern DT_Point PolygonCentroid(const PointArray& polygon);

// �ж϶���εķ���
// ����ֵ:
//		0  -- ����
//		1  -- ˳ʱ��
//	   -1  -- ��ʱ��
extern int CloseWise(const PointArray& polygon);

// �߶εĽ���
// 0��ʾ����״̬(���غϣ��޽���)
// -1��ʾ�н��㵫�����߶���(���ӽ���)
// 1��ʾ�н���
extern int LineIntersect( const DT_Point& a, const DT_Point& b, const DT_Point& c, const DT_Point& d, DT_Point& p);
