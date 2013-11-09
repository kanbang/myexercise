#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// 井下爆炸材料库风量计算
	class UEM_AFCStrategy : public AFCStrategy
	{
	public:
		UEM_AFCStrategy(const CString& tableName);
		~UEM_AFCStrategy(void);

		// 设置井下爆炸材料库的体积
		void setV(double v);

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double V;            // 井下爆炸材料库体积(m3)
	};
}