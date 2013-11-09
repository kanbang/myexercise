#pragma once

#include "AFC_CheckStrategy.h"

namespace PJ
{
	// 按一次爆炸最大炸药量验算风量
	class MaxWeight_AFC_CheckStrategy : public AFC_CheckStrategy
	{
	public:
		MaxWeight_AFC_CheckStrategy(const CString& tableName);

		// 设置一次爆炸炸药最大用量
		void setMaxWeight(double weight);

	protected:
		virtual double minQ();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double maxWeight;            // 一次爆破炸药最大用量(kg)
	};
}