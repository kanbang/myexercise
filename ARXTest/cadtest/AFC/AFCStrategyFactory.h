#pragma once

#include "CoalMineInfo.h"
#include "VelocityInfo.h"

namespace PJ
{
	class AFCStrategy;

	// 风量计算策略对象创建工厂
	class AFCStrategyFactory
	{
	public:
		AFCStrategy* createMC_AFCStrategy(double max_kdj, double min_kdj, double height, double reasonalVelocity, double k_height, double k_length, double k_temperature);
		AFCStrategy* createGas_AFCStrategy(double q_ch4, double k_ch4, double q_ch4_tail = 0);
		AFCStrategy* createTve_AFCStrategy(double reasonalVelocity, double area);
		AFCStrategy* createMaxPeopleNum_AFC_CheckStrategy(int maxPeopleNum);
		AFCStrategy* createMaxWeight_AFC_CheckStrategy(double maxWeight);
		AFCStrategy* createVE_AFC_CheckStrategy(double area, MIN_VELOCITY V_min, MAX_VELOCITY V_max, VelocityCheckType vct);
		AFCStrategy* createLocalFan_AFCStrategy(double area, int fanNum, double q_fan, bool isRock = true);
		AFCStrategy* createUEM_AFCStrategy(double V);
		AFCStrategy* createPCR_AFCStrategy(double q_h2);
		AFCStrategy* createLargeMEC_AFCStrategy(double power, double theta, double delta_t);
		AFCStrategy* createSmallMEC_AFCStrategy(double power, double q_def);
		AFCStrategy* createOT_Gas_AFCStrategy(double q_ch4_max, CoalMineGasInfo gasInfo, TunnelMaterial tm);
		AFCStrategy* createRTC_AFCStrategy(int num, double power);
		AFCStrategy* createConstMinQ_AFC_CheckStrategy();
		AFCStrategy* createNull_AFCStrategy(const CString& tableName);
	};
}