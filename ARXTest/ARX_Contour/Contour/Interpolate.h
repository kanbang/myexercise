#pragma once

#include "GeoDef.h"

extern "C" __declspec(dllexport) 
double InterpolatePoint(const PointArray& datas, const DT_Point& pt);

extern "C" __declspec(dllexport) 
extern void InterpolatePoints(const PointArray& datas, PointArray& pts);
