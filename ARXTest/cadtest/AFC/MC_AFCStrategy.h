#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// 按照气象条件计算风量
	class MC_AFCStrategy : public AFCStrategy
	{
	public:
		MC_AFCStrategy(const CString& tableName);

		// 计算工作面所需的基本风量
		double getBasicQ() const;

		// 设置控顶距
		void setKDJ(double max_kdj, double min_kdj);

		// 获取工作面平均控顶距
		double getAverageKDJ() const;

		// 设置工作面实际采高
		void setHeight(double height);

		// 设置工作面适宜风速
		void setReasonalVelocity(double v);

		// 设置工作面调整系数
		void setAdjustCoefficient(double k_height, double k_length, double k_temperature);

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double max_kdj, min_kdj;	    // 工作面最大和最小控顶距
		double height;                 // 工作面实际采高
		double reasonalVelocity;      // 工作面适宜风速（可能是根据温度选择的适宜风速）
		double k_height;              // 回采工作面采高调整系数
		double k_length;              // 回采工作面长度调整系数
		double k_temperature;        // 回采工作面温度调整系数
	};
}