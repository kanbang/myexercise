#pragma once

#include "GasParam.h"

class GasParamHelper
{
public:
	/*
	 * 如果词典不存在，则创建词典，同时初始化参数
	 */
	static void InitParams();

	static void ReadGasParam(GasParam& gp);

	static void WriteGasParam(const GasParam& gp);
};
