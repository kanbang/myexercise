#pragma once

#include "AFC_CheckStrategy.h"

namespace PJ
{
	// ����������������������
	class MaxPeopleNum_AFC_CheckStrategy : public AFC_CheckStrategy
	{
	public:
		MaxPeopleNum_AFC_CheckStrategy(const CString& tableName);

		// ���ù������������
		void setMaxPeopleNum(int num);

	protected:
		virtual double minQ(); 
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		int maxPeopleNum;            // �زɣ�����)�������������(��)
	};
}