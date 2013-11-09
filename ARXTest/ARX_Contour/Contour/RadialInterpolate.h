#pragma once

#include "GeoDef.h"

extern double RadialInterpolatePoint(const PointArray& datas, const DT_Point& pt);
extern void RadialInterpolatePoints(const PointArray& datas, PointArray& pts);
