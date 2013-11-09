#pragma once

#include "../DaoHelper/DaoData.h"
#include <deque>

namespace PJ
{
	class AFCStrategy;

	// 风量计算策略组合器
	class AFCStrategyComposite : public DaoData
	{
	public:
		AFCStrategyComposite(const CString& tableName);

		void setUserQ(double q);
		double getMaxQ() const;
		void regAFCStrategy(AFCStrategy* pAFCStrategy);
		void clearAllAFCStrategies();

	protected:
		virtual void doCreateTable(TableCreator* pTableCreator);
		virtual void doWriteToTable(DataWriter* pDataWriter);

		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		typedef std::deque<AFCStrategy*> AFCStrategyCollection;
		AFCStrategyCollection afcs;
		double maxQ;    // 计算得到的最大风量
		double userQ;   // 用户指定的风量
	};
}