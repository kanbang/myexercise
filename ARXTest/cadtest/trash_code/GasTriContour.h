#pragma once

#include "TriContour.h"

// 只起到分类的作用

// 瓦斯涌出等值线
class GasFlowTriContour : public TriContour
{
public:
	ACRX_DECLARE_MEMBERS(GasFlowTriContour);

public:
	GasFlowTriContour();
};

// 瓦斯含量等值线
class GasContentTriContour : public TriContour
{
public:
	ACRX_DECLARE_MEMBERS(GasContentTriContour);

public:
	GasContentTriContour();
};

// 瓦斯压力等值线
class GasPressureTriContour : public TriContour
{
public:
	ACRX_DECLARE_MEMBERS(GasPressureTriContour);

public:
	GasPressureTriContour();
};


#ifdef CONTOUR_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GasFlowTriContour)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GasContentTriContour)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GasPressureTriContour)
#endif
