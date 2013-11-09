#pragma once

namespace PJ
{
	// 评价插件加载结果
	enum EvalPluginLoadResult
	{
		EPLR_ILLEGAL_EVAL_PLUGIN = 0,       // 非法的评价插件
		EPLR_LOAD_SUCCESS,                  // 加载成功
		EPLR_CAN_NOT_CREATE_PLUGIN,         // 无法创建插件
		EPLR_HAS_LOAD                       // 插件已经加载
	};
}