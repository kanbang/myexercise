#pragma once

#include "GeoDef.h"

extern double KrigingInterpolatePoint(const PointArray& datas, const DT_Point& pt);
extern void KrigingInterpolatePoints(const PointArray& datas, PointArray& pts);
