#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// 井下充电室风量计算
	class PCR_AFCStrategy : public AFCStrategy
	{
	public:
		PCR_AFCStrategy(const CString& tableName);

		// 设置充电时产生的氢气量
		void setQ_H2(double q);

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double q_h2;        // 充电时产生的氢气量
	};
}