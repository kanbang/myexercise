#pragma once

#include "AFCStrategy.h"

namespace PJ
{
	// 回采工作面或掘进工作面按瓦斯涌出量计算风量
	// 同时考虑了配备专排巷的工作面的风量计算
	class Gas_AFCStrategy : public AFCStrategy
	{
	public:
		Gas_AFCStrategy(const CString& tableName);

		// 设置瓦斯平均绝对涌出量
		void setQ_CH4(double q);

		// 设置瓦斯涌出不均衡通风系数
		void setK_CH4(double k);

		// 设置专排巷瓦斯涌出量，如果没有配备专排巷，那么设置为0
		void setQ_CH4_TAIL(double q = 0);

	protected:
		virtual double caculate();
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		double q_ch4;             // 回风巷风流中瓦斯（或二氧化碳）的平均绝对涌出量
		double k_ch4;             // 瓦斯涌出不均衡通风系数
		double q_ch4_tail;       // 专排巷的瓦斯涌出量（如果配备了专排瓦斯巷），默认没有配备专排巷
	};
}