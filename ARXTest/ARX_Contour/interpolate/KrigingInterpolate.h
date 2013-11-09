#pragma once

extern double KrigingInterpolatePoint(const AcGePoint3dArray& datas, const AcGePoint3d& pt);

extern void KrigingInterpolatePoints(const AcGePoint3dArray& datas, AcGePoint3dArray& pts);
