#pragma once

#include "../DaoHelper/DaoData.h"

namespace PJ
{
	class AFCStrategyComposite;

	// 风量计算结果汇总
	class AFCResult : public DaoData
	{
	public:
		AFCResult(const CString& tableName);

		double getMaxQ() const;
		void regAFCStrategyComposite(const CString& name, double userQ, AFCStrategyComposite* pAFCStrategyComposite);

	protected:
		virtual void doCreateTable(TableCreator* pTableCreator);
		virtual void doWriteToTable(DataWriter* pDataWriter);

		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		AFCStrategyComposite* pAFCStrategyComposite;
		CString name;  // 图元的名称(用风地点：xx工作面, xxx掘进面等)
		double maxQ;   // 获取通过计算得到的最大需风量
		double userQ;  // 用户指定的风量
	};
}