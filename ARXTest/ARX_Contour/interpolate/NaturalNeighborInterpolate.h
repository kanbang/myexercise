#pragma once

extern double NaturalNeighInterpolatePoint(const AcGePoint3dArray& datas, const AcGePoint3d& pt);

extern void NaturalNeighInterpolatePoints(const AcGePoint3dArray& datas, AcGePoint3dArray& pts);

extern double LinearInterpolatePoint(const AcGePoint3dArray& datas, const AcGePoint3d& pt);

extern void LinearInterpolatePoints(const AcGePoint3dArray& datas, AcGePoint3dArray& pts);
