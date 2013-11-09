#pragma once

#include "dlimexp.h"

namespace PJ
{
	// 评价结果
	enum EvalResult
	{
		UnQualified     = 0,     // 不合格
		Qualified       = 1,     // 合格
		NoneResult      = 2,      // 无该项评价(条款评价已实现，但没有数据可以评价)
		/*
		 * 该评价结果只是一种临时过滤状态
		 * 用于一些特殊情况的判断
		 * 最终生成的数据库中没有该类型的结果
		 */
		UnknownResult   = 3,     // 未知结果(条款评价未实现，需人工询问)
	};

	// 将布尔值转换成EvalResult，不考虑UNKNOWN_RESULT的情况
	// 只是为了方便使用
	extern "C" EVAL_BASE_EXPORT_API EvalResult ConvertBoolToEvalResult(bool value);
}