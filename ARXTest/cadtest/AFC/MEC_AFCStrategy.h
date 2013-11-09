#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	class MEC_AFCStrategy : public AFCStrategy
	{
	public:
		MEC_AFCStrategy(const CString& tableName);

		// �������ҵ��豸�Ĺ���
		void setPower(double power);

		// ���û������ҵķ���ϵ��
		void setTheta(double theta);

		// ���ý��ط������¶Ȳ�
		void setDeltaT(double delta_t);

		// �趨�Ƿ�Ϊ�������������
		void setMECSize(bool isLargeMEC);

		// С�����Ҹ��ݾ���ָ������
		void setDefQ(double q);

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double power;             // �����豸�Ĺ���
		bool isLargeMEC;          // �Ƿ�������Ļ������ң�Ĭ��Ϊ���ͻ������ң�

		double theta;             // �������ҵķ���ϵ��
		double rho;              // �����ܶȣ�һ��ȡ.20kg/m3���̶��ģ����Բ���Ҫ��
		double c_p;              // ������ѹ���ȣ�һ��ȡ.0006KJ/kg.k���̶��ģ�
		double delta_t;         // �������ҽ����ط������¶Ȳ�(���϶�)

		double q_def;           // С�����Ҹ��ݾ���ָ���ķ���
	};
}