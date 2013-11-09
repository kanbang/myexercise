#pragma once

// 瓦斯等值线类型
enum GasContourType
{
	GCT_FLOW     = 0,   // 绝对瓦斯涌出量(GFP)
	GCT_CONTENT  = 1,   // 瓦斯含量(GCP)
	GCT_PRESSURE = 2    // 瓦斯压力值(GPP)
};

extern CString GetGasTypeName(GasContourType gct);
extern CString GetGasFieldName(GasContourType gct);
