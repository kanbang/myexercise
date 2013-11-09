#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// ���±�ը���Ͽ��������
	class UEM_AFCStrategy : public AFCStrategy
	{
	public:
		UEM_AFCStrategy(const CString& tableName);
		~UEM_AFCStrategy(void);

		// ���þ��±�ը���Ͽ�����
		void setV(double v);

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double V;            // ���±�ը���Ͽ����(m3)
	};
}