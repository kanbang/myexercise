#pragma once

#include "AFC_CheckStrategy.h"
#include "VelocityInfo.h"

namespace PJ
{
	// 风速验算
	class AFC_VelocityCheckStrategy : public AFC_CheckStrategy
	{
	public:
		AFC_VelocityCheckStrategy(const CString& tableName);

		// 设置断面面积
		void setArea(double area);

		// 设置风速的上下限
		void setMinMaxVelocity(MIN_VELOCITY V_min, MAX_VELOCITY V_max);

		// 设置风速验算类型(设计的不太好，目前也没有好办法）
		void setVelocityCheckType(VelocityCheckType vct);

	protected:
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

		virtual double minQ();
		virtual double maxQ();

	private:
		double getMinVelocity() const;
		double getMaxVelocity() const;
		double area; // 断面面积
		MIN_VELOCITY V_min; // 最小风速（默认为NORMAL_MIN_VELOCITY--0.25m/s)
		MAX_VELOCITY V_max; // 最大风速（默认为NORMAL_MAX_VELOCITY--4m/s)
		VelocityCheckType vct; // 风速验算类型
	};
}