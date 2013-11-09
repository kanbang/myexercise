#pragma once

#include "dlimexp.h"

namespace PJ
{
	// 评价插件描述信息
	struct EVAL_BASE_EXPORT_API EvalPluginInfo
	{
		CString name;              // 规程名称--数据库中的信息表名称
		CString author;            // 开发者
		CString date;              // 开发时间
		CString regulation;        // 规程名称--实际名称(例如"煤矿安全规程2004")
		CString clauseImpl;		   // 已实现的条款范围
	};
}