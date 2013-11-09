#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// ��ֵ����
	class NullAFCStrategy : public AFCStrategy
	{
	public:
		NullAFCStrategy(const CString& tableName);

	protected:
		virtual void doCreateTable(TableCreator* pTableCreator);
		virtual void doWriteToTable(DataWriter* pDataWriter);

		virtual double caculate();
	};
}