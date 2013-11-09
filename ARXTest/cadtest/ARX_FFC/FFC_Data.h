#pragma once

struct ObturationData
{
	double q;
};

struct InletData
{
	double area;
	double q;
	double ch4;
	double o2;
};

struct AirLeakData
{
	double q;
};

struct GasPipeData
{
	double q;
	double ch4;
	double o2;
};

struct NitrogenPipeData
{
	double n2;
};

// 采空区瓦斯边界
struct GasBoundaryData
{
	double q;
};

typedef AcArray<ObturationData> ObturationDataArray;
typedef AcArray<InletData> InletDataArray;
typedef AcArray<AirLeakData> AirLeakDataArray;
typedef AcArray<GasPipeData> GasPipeDataArray;
typedef AcArray<NitrogenPipeData> NitrogenPipeDataArray;
typedef AcArray<GasBoundaryData> GasBoundaryDataArray;