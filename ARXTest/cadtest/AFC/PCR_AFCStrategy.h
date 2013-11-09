#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// ���³���ҷ�������
	class PCR_AFCStrategy : public AFCStrategy
	{
	public:
		PCR_AFCStrategy(const CString& tableName);

		// ���ó��ʱ������������
		void setQ_H2(double q);

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double q_h2;        // ���ʱ������������
	};
}