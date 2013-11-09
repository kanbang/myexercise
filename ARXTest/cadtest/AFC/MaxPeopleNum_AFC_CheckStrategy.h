#pragma once

#include "AFC_CheckStrategy.h"

namespace PJ
{
	// 按工作面最多人数验算风量
	class MaxPeopleNum_AFC_CheckStrategy : public AFC_CheckStrategy
	{
	public:
		MaxPeopleNum_AFC_CheckStrategy(const CString& tableName);

		// 设置工作面最多人数
		void setMaxPeopleNum(int num);

	protected:
		virtual double minQ(); 
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		int maxPeopleNum;            // 回采（或掘进)工作面最多人数(人)
	};
}