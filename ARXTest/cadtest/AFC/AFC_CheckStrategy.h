#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// ��������
	class AFC_CheckStrategy : public AFCStrategy
	{
	protected:
		AFC_CheckStrategy(const CString& tableName);
		virtual double caculate(); // �������

		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

		virtual double minQ(); // ��С������Ĭ��Ϊ0��
		virtual double maxQ(); // ��������Ĭ��Ϊ10000��
	};
}