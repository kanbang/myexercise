#pragma once

#include "../DaoHelper/DaoData.h"

namespace PJ
{
	class AFCStrategyComposite;

	// ��������������
	class AFCResult : public DaoData
	{
	public:
		AFCResult(const CString& tableName);

		double getMaxQ() const;
		void regAFCStrategyComposite(const CString& name, double userQ, AFCStrategyComposite* pAFCStrategyComposite);

	protected:
		virtual void doCreateTable(TableCreator* pTableCreator);
		virtual void doWriteToTable(DataWriter* pDataWriter);

		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		AFCStrategyComposite* pAFCStrategyComposite;
		CString name;  // ͼԪ������(�÷�ص㣺xx������, xxx������)
		double maxQ;   // ��ȡͨ������õ�����������
		double userQ;  // �û�ָ���ķ���
	};
}