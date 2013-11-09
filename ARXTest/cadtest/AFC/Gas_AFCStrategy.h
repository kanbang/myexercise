#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// �زɹ�������������水��˹ӿ�����������
	// ͬʱ�������䱸ר����Ĺ�����ķ�������
	class Gas_AFCStrategy : public AFCStrategy
	{
	public:
		Gas_AFCStrategy(const CString& tableName);

		// ������˹ƽ������ӿ����
		void setQ_CH4(double q);

		// ������˹ӿ��������ͨ��ϵ��
		void setK_CH4(double k);

		// ����ר������˹ӿ���������û���䱸ר�����ô����Ϊ0
		void setQ_CH4_TAIL(double q = 0);

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double q_ch4;             // �ط����������˹���������̼����ƽ������ӿ����
		double k_ch4;             // ��˹ӿ��������ͨ��ϵ��
		double q_ch4_tail;       // ר�������˹ӿ����������䱸��ר����˹���Ĭ��û���䱸ר����
	};
}