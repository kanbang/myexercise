#pragma once

#include "../DaoHelper/DaoData.h"

namespace PJ
{
	class AFCStrategy : public DaoData
	{
	public:
		virtual ~AFCStrategy() {}

		void setLastQ(double lastQ); // 设置上次计算的风量
		double getMaxQ();            // 计算风量，并和上次计算的风量进行比较，得到较大的风量

	protected:
		AFCStrategy(const CString& tableName);
		void setCaculateQ(double q);
		double getLastQ() const;

		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);
		virtual double caculate() = 0; // 计算风量

	protected:
		double m_lastQ;     // 上次计算的风量
		double m_caculateQ; // 计算的风量
	};
}