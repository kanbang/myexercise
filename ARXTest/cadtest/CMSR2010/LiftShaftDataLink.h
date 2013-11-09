#pragma once

#include "../MineGE/DataLink.h"

// 用作提升的井筒(巷道)数据链接
class LiftShaftDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;         // 名称
	double v;             // 风速
	int ft;              // 功能类型
	int sp;              // 井筒附加属性

	int vt;              // 通风类型(非字段,由程序赋值)
	bool hasLift;        // 有无提升煤炭的设备
};
