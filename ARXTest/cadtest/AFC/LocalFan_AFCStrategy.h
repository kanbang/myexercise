#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// 掘进工作面按局部通风机计算风量
	class LocalFan_AFCStrategy : public AFCStrategy
	{
	public:
		LocalFan_AFCStrategy(const CString& tableName);

		// 设置断面面积
		void setArea(double area);

		// 设置局部通风机台数
		void setFanNum(int num);

		// 设置局部通风机实际吸风量
		void setQ_FAN(double q);

		// 设置是否岩巷
		void changeTunnelType(bool isRock = false);

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double area;             // 断面面积
		int fanNum;              // 通风机台数
		double q_fan;            // 局部通风机实际吸风量
		bool isRock;             // 是否岩巷（默认为非岩巷）
	};
}