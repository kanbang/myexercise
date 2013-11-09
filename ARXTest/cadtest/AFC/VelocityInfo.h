#pragma once

namespace PJ
{
	// 最小风速取值
	enum MIN_VELOCITY
	{
		SMALL_MIN_VELOCITY,  // 0.15m/s
		NORMAL_MIN_VELOCITY, // 0.25m/s
		LARGE_MIN_VELOCITY,  // 0.5m/s
		MORE_MIN_VELOCITY    // 1.0m/s
	};

	// 最大风速取值
	enum MAX_VELOCITY
	{
		NORMAL_MAX_VELOCITY, // 4m/s
		LARGE_MAX_VELOCITY,  // 5m/s
		NO_LIMIT_MAX_VELOCITY // 无限制的最大风速，这里设定为100m/s
	};

	// 风速验算类型
	enum VelocityCheckType
	{
		VCT_WS_NORMAL=0,  // 回采工作面风速验算6(1)
		VCT_WS_GASTUNNEL, // 回采工作面风速验算6(3)，配备专排巷
		VCT_ROCK,     // 岩巷风速验算
		VCT_COAL,     // 煤巷风速验算
		VCT_EL        // 架线电车机巷风速验算
	};
}