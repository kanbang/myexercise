#pragma once

#include "../DaoHelper/DaoData.h"

namespace PJ
{
	class AFCStrategy : public DaoData
	{
	public:
		virtual ~AFCStrategy() {}

		void setLastQ(double lastQ); // �����ϴμ���ķ���
		double getMaxQ();            // ��������������ϴμ���ķ������бȽϣ��õ��ϴ�ķ���

	protected:
		AFCStrategy(const CString& tableName);
		void setCaculateQ(double q);
		double getLastQ() const;

		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);
		virtual double caculate() = 0; // �������

	protected:
		double m_lastQ;     // �ϴμ���ķ���
		double m_caculateQ; // ����ķ���
	};
}