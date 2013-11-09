#pragma once

#include "GeoDef.h"

extern double NaturalNeighInterpolatePoint(const PointArray& datas, const DT_Point& pt);
extern void NaturalNeighInterpolatePoints(const PointArray& datas, PointArray& pts);
extern double LinearInterpolatePoint(const PointArray& datas, const DT_Point& pt);
extern void LinearInterpolatePoints(const PointArray& datas, PointArray& pts);
