#pragma once

#include "AFC_CheckStrategy.h"

namespace PJ
{
	// ��С�����ǹ̶�ֵ������Ҫ����
	class ConstMinQ_AFC_CheckStrategy : public AFC_CheckStrategy
	{
	public:
		ConstMinQ_AFC_CheckStrategy(const CString& tableName);

	protected:
		virtual double minQ();
	};
}