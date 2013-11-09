#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// 根据工作面温度选择适宜风速计算风量
	class TVE_AFCStrategy : public AFCStrategy
	{
	public:
		TVE_AFCStrategy(const CString& tableName);
		~TVE_AFCStrategy(void);

		//  设置根据温度选择的适宜风速
		void setReasonalVelocity(double v);

		// 设置断面面积
		void setArea(double area);

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double reasonalVelocity;     // 根据温度选择的风速(m/s)
		double area;                  // 回采工作面的平均断面积(m2)
	};
}