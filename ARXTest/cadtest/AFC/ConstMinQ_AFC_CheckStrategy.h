#pragma once

#include "AFC_CheckStrategy.h"

namespace PJ
{
	// 最小风量是固定值，不需要计算
	class ConstMinQ_AFC_CheckStrategy : public AFC_CheckStrategy
	{
	public:
		ConstMinQ_AFC_CheckStrategy(const CString& tableName);

	protected:
		virtual double minQ();
	};
}