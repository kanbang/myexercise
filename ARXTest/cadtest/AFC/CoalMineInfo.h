#pragma once

namespace PJ
{
	// 巷道材料类型
	enum TunnelMaterial
	{
		ROCK   = 0, // 岩巷
		COAL   = 1, // 煤巷
		MIXED  = 2  // 半煤岩巷
	};

	// 矿井瓦斯信息
	enum CoalMineGasInfo
	{
		LOW_GAS  = 0, // 低瓦斯矿井
		HIGH_GAS = 1, // 高瓦斯矿井
		OUTBURST = 2  // 煤与瓦斯突出
	};
}