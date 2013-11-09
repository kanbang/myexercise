#pragma once

#include "../MineGE/DataLink.h"

// 采掘工作面模型--煤安第100条
class TWSDataLink : public DataLink
{
protected:
	virtual void regDatas();

	// 为了简单起见，所有的数据字段都为public类型，便于访问
public:
	CString name;      // 名称
	double O2;         // 氧气浓度
	double CO2;        // 二氧化碳浓度
	double CO;         // 一氧化碳浓度
	double NO;         // 氧化氮浓度（在煤安的第100条中有涉及到）
	double SO2;        // 二氧化硫浓度（同上）
	double H2S;        // 硫化氢浓度（同上）
	double NH3;        // 氨浓度（同上）
};
