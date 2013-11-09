#pragma once

#define FFC_GAMBIT_PARAM_DICT _T("Gambit���ν�ģ�����ʵ�")
#define FFC_GAMBIT_WIDTH_KEY _T("���������")

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
