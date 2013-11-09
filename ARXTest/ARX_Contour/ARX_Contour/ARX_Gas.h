#pragma once

#include "GasContourType.h"

extern void GetEntityLayers(const CString& type, AcStringArray& layers);
extern void GetInsertPoints(const AcDbObjectIdArray& objIds, AcGePoint3dArray& pts);
extern void GetGasDatas(GasContourType gas_type, AcGePoint3dArray& datas);
extern void GetBoundaryPoints(AcGePoint3dArray& pts);
