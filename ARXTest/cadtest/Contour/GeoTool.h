#pragma once

#include "GeoDef.h"

// 判断浮点数是否相等
extern bool IsDoubleEqual(double a, double b);

// 判断是否有等值点
// 1 -- 有  -1) 没有 0 -- 与z1或z2相等
extern int InRange(double z1, double z2, double z);

// 线性插值
extern void LinearInterpolate(const DT_Point& p1, const DT_Point& p2, DT_Point& p);

// 判断两点是否相同(不考虑z值)
extern bool IsPointEqual(const DT_Point& p1, const DT_Point& p2);

// 判断点是否在多边形内
extern bool IsPointInPolygon (const DT_Point& p, const PointArray& ptPolygon) ;

// 判断点是否在线段上
extern bool IsPointOnline(const DT_Point& p1, const DT_Point& p2, const DT_Point& p);

// 两点之间的中点
extern DT_Point MidPoint(const DT_Point& p1, const DT_Point& p2);

// 两点构成的直线角度方向
extern double Direction(const DT_Point& p1, const DT_Point& p2);

// 两点距离(不开平方，提高效率)
extern double Distance(const DT_Point& p1, const DT_Point& p2);

// 两点距离的平方
extern double Distance_2(const DT_Point& p1, const DT_Point& p2);

// 计算点p到直线(p1, p2)的距离
extern double GetPointDistanceToLine(const DT_Point& p, const DT_Point& p1, const DT_Point& p2);

// 计算多边形面积
extern double PolygonArea(const PointArray& polygon);
