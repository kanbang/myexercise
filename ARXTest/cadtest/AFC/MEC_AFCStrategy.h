#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	class MEC_AFCStrategy : public AFCStrategy
	{
	public:
		MEC_AFCStrategy(const CString& tableName);

		// 设置硐室的设备的功率
		void setPower(double power);

		// 设置机电硐室的发热系数
		void setTheta(double theta);

		// 设置进回风流的温度差
		void setDeltaT(double delta_t);

		// 设定是否为发热量大的硐室
		void setMECSize(bool isLargeMEC);

		// 小型硐室根据经验指定风量
		void setDefQ(double q);

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double power;             // 硐室设备的功率
		bool isLargeMEC;          // 是否发热量大的机电硐室（默认为大型机电硐室）

		double theta;             // 机电硐室的发热系数
		double rho;              // 空气密度，一般取.20kg/m3（固定的，可以不需要）
		double c_p;              // 空气定压比热，一般取.0006KJ/kg.k（固定的）
		double delta_t;         // 机电硐室进、回风流测温度差(摄氏度)

		double q_def;           // 小型硐室根据经验指定的风量
	};
}