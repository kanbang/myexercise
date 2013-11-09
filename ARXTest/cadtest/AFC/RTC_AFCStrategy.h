#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	class RTC_AFCStrategy : public AFCStrategy
	{
	public:
		RTC_AFCStrategy(const CString& tableName);

		// ���ý��ֳ���̨��
		void setRTCNum(int num);

		// ���ý��ֳ�����
		void setRTCPower(double power);

		// ���ݽ��ֳ�̨���������ϵ��
		double getK() const;

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		int num;           // ��i���ص�Ľ��ֳ���̨��
		double power;      // ��i���ص�Ľ��ֳ��Ĺ���(kw)
	};
}