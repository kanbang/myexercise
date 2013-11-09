#pragma once

#include "TriContour.h"

// ֻ�𵽷��������

// ��˹ӿ����ֵ��
class GasFlowTriContour : public TriContour
{
public:
	ACRX_DECLARE_MEMBERS(GasFlowTriContour);

public:
	GasFlowTriContour();
};

// ��˹������ֵ��
class GasContentTriContour : public TriContour
{
public:
	ACRX_DECLARE_MEMBERS(GasContentTriContour);

public:
	GasContentTriContour();
};

// ��˹ѹ����ֵ��
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
