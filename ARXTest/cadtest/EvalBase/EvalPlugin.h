#pragma once

#include "HelperMacro.h"
#include "EvalMethod.h"
#include "EvalPluginInfo.h"

namespace PJ
{
	// 评价插件接口
	class EvalPlugin
	{
	public:
		virtual ~EvalPlugin() {}

		// 初始化
		virtual void initialize(EvalMethod* pEvalMethod) = 0;

		// 卸载清理
		virtual void unInitialize(EvalMethod* pEvalMethod) = 0;

		// 获取插件信息
		virtual void getPluginInfo(EvalPluginInfo& info) const = 0;
	};

	// 评价插件创建函数签名
	typedef EvalPlugin* (*EvalPluginCreateFunc)();
}