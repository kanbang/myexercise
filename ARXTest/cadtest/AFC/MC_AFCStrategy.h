#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// �������������������
	class MC_AFCStrategy : public AFCStrategy
	{
	public:
		MC_AFCStrategy(const CString& tableName);

		// ���㹤��������Ļ�������
		double getBasicQ() const;

		// ���ÿض���
		void setKDJ(double max_kdj, double min_kdj);

		// ��ȡ������ƽ���ض���
		double getAverageKDJ() const;

		// ���ù�����ʵ�ʲɸ�
		void setHeight(double height);

		// ���ù��������˷���
		void setReasonalVelocity(double v);

		// ���ù��������ϵ��
		void setAdjustCoefficient(double k_height, double k_length, double k_temperature);

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double max_kdj, min_kdj;	    // ������������С�ض���
		double height;                 // ������ʵ�ʲɸ�
		double reasonalVelocity;      // ���������˷��٣������Ǹ����¶�ѡ������˷��٣�
		double k_height;              // �زɹ�����ɸߵ���ϵ��
		double k_length;              // �زɹ����泤�ȵ���ϵ��
		double k_temperature;        // �زɹ������¶ȵ���ϵ��
	};
}