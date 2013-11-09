#pragma once

#include "AFC_CheckStrategy.h"
#include "VelocityInfo.h"

namespace PJ
{
	// ��������
	class AFC_VelocityCheckStrategy : public AFC_CheckStrategy
	{
	public:
		AFC_VelocityCheckStrategy(const CString& tableName);

		// ���ö������
		void setArea(double area);

		// ���÷��ٵ�������
		void setMinMaxVelocity(MIN_VELOCITY V_min, MAX_VELOCITY V_max);

		// ���÷�����������(��ƵĲ�̫�ã�ĿǰҲû�кð취��
		void setVelocityCheckType(VelocityCheckType vct);

	protected:
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

		virtual double minQ();
		virtual double maxQ();

	private:
		double getMinVelocity() const;
		double getMaxVelocity() const;
		double area; // �������
		MIN_VELOCITY V_min; // ��С���٣�Ĭ��ΪNORMAL_MIN_VELOCITY--0.25m/s)
		MAX_VELOCITY V_max; // �����٣�Ĭ��ΪNORMAL_MAX_VELOCITY--4m/s)
		VelocityCheckType vct; // ������������
	};
}