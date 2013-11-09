#pragma once

#include "../MineGE/DataLink.h"

// 图元类型
enum TEMP_TYPE
{
	TEMP_COMMON = 1,    // 普通巷道(非进风井巷道、火药库、充电室)--后续可能废除
	TEMP_TWS    = 2,    // 采掘工作面
	TEMP_MR     = 3,    // 机电硐室
	TEMP_MR2    = 4     // 其它硐室(火药库、充电室) -- 目前的实现中没有考虑风桥,后续可能废除风桥
};

// 温度模型
class TempDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	TEMP_TYPE type;          // 类型
	CString name;            // 名称
	double t;                // 温度	
	int cm;                  // 超温控制措施
};
