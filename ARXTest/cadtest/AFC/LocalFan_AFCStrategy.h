#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// ��������水�ֲ�ͨ����������
	class LocalFan_AFCStrategy : public AFCStrategy
	{
	public:
		LocalFan_AFCStrategy(const CString& tableName);

		// ���ö������
		void setArea(double area);

		// ���þֲ�ͨ���̨��
		void setFanNum(int num);

		// ���þֲ�ͨ���ʵ��������
		void setQ_FAN(double q);

		// �����Ƿ�����
		void changeTunnelType(bool isRock = false);

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double area;             // �������
		int fanNum;              // ͨ���̨��
		double q_fan;            // �ֲ�ͨ���ʵ��������
		bool isRock;             // �Ƿ����Ĭ��Ϊ�����
	};
}