#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// ���ݹ������¶�ѡ�����˷��ټ������
	class TVE_AFCStrategy : public AFCStrategy
	{
	public:
		TVE_AFCStrategy(const CString& tableName);
		~TVE_AFCStrategy(void);

		//  ���ø����¶�ѡ������˷���
		void setReasonalVelocity(double v);

		// ���ö������
		void setArea(double area);

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double reasonalVelocity;     // �����¶�ѡ��ķ���(m/s)
		double area;                  // �زɹ������ƽ�������(m2)
	};
}