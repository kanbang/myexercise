#pragma once

#include "../Contour/GeoAlgo.h"

typedef AcArray<COLORREF> AcGeColorArray;

extern double ARX_InterpolatePoint(const AcGePoint3dArray& datas, const AcGePoint3d& pt);
extern void ARX_InterpolatePoints(const AcGePoint3dArray& datas, AcGePoint3dArray& pts);

extern void ARX_GetMinMaxZValue(const AcGePoint3dArray& datas, double& minZ, double& maxZ);
extern void ARX_PreprocessPointSet(AcGePoint3dArray& bounds, const AcGePoint3dArray& datas, AcGePoint3dArray& new_datas);
extern void ARX_DrawCountour(AcGePoint3dArray& bounds, const AcGePoint3dArray& pts, double minZ, double maxZ, double dz);
extern void ARX_DrawFill(AcGePoint3dArray& bounds, const AcGePoint3dArray& pts, const AcGeDoubleArray& zValues, const AcGeColorArray& colors);