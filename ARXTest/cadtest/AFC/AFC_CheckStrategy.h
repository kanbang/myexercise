#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// 风量验算
	class AFC_CheckStrategy : public AFCStrategy
	{
	protected:
		AFC_CheckStrategy(const CString& tableName);
		virtual double caculate(); // 计算风量

		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

		virtual double minQ(); // 最小风量（默认为0）
		virtual double maxQ(); // 最大风量（默认为10000）
	};
}