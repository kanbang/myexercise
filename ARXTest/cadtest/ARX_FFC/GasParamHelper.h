#pragma once

#include "GasParam.h"

class GasParamHelper
{
public:
	/*
	 * ����ʵ䲻���ڣ��򴴽��ʵ䣬ͬʱ��ʼ������
	 */
	static void InitParams();

	static void ReadGasParam(GasParam& gp);

	static void WriteGasParam(const GasParam& gp);
};
