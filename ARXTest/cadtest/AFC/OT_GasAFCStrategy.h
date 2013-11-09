#pragma once

#include "AFCStrategy.h"
#include "CoalMineInfo.h"

namespace PJ
{
	// �����������˹ӿ�����������
	class OT_GasAFCStrategy : public AFCStrategy
	{
	public:
		OT_GasAFCStrategy(const CString& tableName);

		// ���������˹����ӿ����
		void setQ_CH4_Max(double q);

		// ���ÿ���˹��Ϣ�Լ������������
		void setInfo(CoalMineGasInfo gasInfo, TunnelMaterial tm);

	protected:
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);
		virtual double caculate();

	private:
		double q_ch4_max; //������������˹����ӿ����
		double k_ch4;     // ��˹ӿ��������ϵ��(�̶�ֵ)
	};
}