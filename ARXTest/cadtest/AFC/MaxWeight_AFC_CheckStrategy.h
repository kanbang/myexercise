#pragma once

#include "AFC_CheckStrategy.h"

namespace PJ
{
	// ��һ�α�ը���ըҩ���������
	class MaxWeight_AFC_CheckStrategy : public AFC_CheckStrategy
	{
	public:
		MaxWeight_AFC_CheckStrategy(const CString& tableName);

		// ����һ�α�ըըҩ�������
		void setMaxWeight(double weight);

	protected:
		virtual double minQ();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double maxWeight;            // һ�α���ըҩ�������(kg)
	};
}