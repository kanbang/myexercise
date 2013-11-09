#pragma once

#include "../DaoHelper/DaoData.h"
#include <deque>

namespace PJ
{
	class AFCStrategy;

	// ����������������
	class AFCStrategyComposite : public DaoData
	{
	public:
		AFCStrategyComposite(const CString& tableName);

		void setUserQ(double q);
		double getMaxQ() const;
		void regAFCStrategy(AFCStrategy* pAFCStrategy);
		void clearAllAFCStrategies();

	protected:
		virtual void doCreateTable(TableCreator* pTableCreator);
		virtual void doWriteToTable(DataWriter* pDataWriter);

		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		typedef std::deque<AFCStrategy*> AFCStrategyCollection;
		AFCStrategyCollection afcs;
		double maxQ;    // ����õ���������
		double userQ;   // �û�ָ���ķ���
	};
}