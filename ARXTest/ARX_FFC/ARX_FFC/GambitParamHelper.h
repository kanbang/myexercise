#pragma once

#define FFC_GAMBIT_PARAM_DICT _T("Gambit几何建模参数词典")
#define FFC_GAMBIT_WIDTH_KEY _T("输出巷道宽度")

#define DEF_FFC_GAMBIT_WIDTH 8

struct GambitParam
{
	double width;
};

class GambitParamHelper
{
public:
	static void InitParams();
	static void ReadGambitParam(GambitParam& gp);
	static void WriteGambitParam(const GambitParam& gp);
};
