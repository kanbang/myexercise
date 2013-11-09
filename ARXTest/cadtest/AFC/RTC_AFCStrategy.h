#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	class RTC_AFCStrategy : public AFCStrategy
	{
	public:
		RTC_AFCStrategy(const CString& tableName);

		// 设置胶轮车的台数
		void setRTCNum(int num);

		// 设置胶轮车功率
		void setRTCPower(double power);

		// 根据胶轮车台数计算配风系数
		double getK() const;

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		int num;           // 第i个地点的胶轮车的台数
		double power;      // 第i个地点的胶轮车的功率(kw)
	};
}