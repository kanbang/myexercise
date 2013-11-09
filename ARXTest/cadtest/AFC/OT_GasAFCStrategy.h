#pragma once

#include "AFCStrategy.h"
#include "CoalMineInfo.h"

namespace PJ
{
	// 其它巷道按瓦斯涌出量计算风量
	class OT_GasAFCStrategy : public AFCStrategy
	{
	public:
		OT_GasAFCStrategy(const CString& tableName);

		// 设置最大瓦斯绝对涌出量
		void setQ_CH4_Max(double q);

		// 设置矿井瓦斯信息以及巷道材料类型
		void setInfo(CoalMineGasInfo gasInfo, TunnelMaterial tm);

	protected:
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);
		virtual double caculate();

	private:
		double q_ch4_max; //其它巷道最大瓦斯绝对涌出量
		double k_ch4;     // 瓦斯涌出不均衡系数(固定值)
	};
}