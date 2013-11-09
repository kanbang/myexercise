#pragma once

#include "../MineGE/DataLink.h"

// 图元类型
enum VD_TYPE
{
	VD_TUNNEL = 1, // 巷道
	VD_WS     = 2, // 回采工作面
	VD_TWS    = 3, // 掘进工作面
	VD_BRIDGE = 4 // 风桥 -- 后续可能废除
	//VD_ROOM   = 5  // 机电硐室
};

// 风速模型--煤安第101条
// 图元通用数据：名称、风速、瓦斯涌出量
class VelocityDataLink : public DataLink
{
protected:
	virtual void regDatas();

protected:
	VelocityDataLink();

public:
	VD_TYPE type;         // 图元类型
	CString name;         // 名称
	double v;             // 风速
	double q_ch4;         // 平均绝对瓦斯涌出量
};

class TunnelVelocityDataLink : public VelocityDataLink
{
public:
	TunnelVelocityDataLink();

protected:
	virtual void regDatas();

public:
	int ft;               // 巷道功能类型
	int sp;               // 井筒附加属性
	bool isShaft;         // 是否井筒
};

// 回采工作面模型
class WSVelocityDataLink : public VelocityDataLink
{
public:
	WSVelocityDataLink();

protected:
	virtual void regDatas();

public:
	int tm;    // 材质类型
	int cm;    // 采煤方法
};

// 掘进工作面模型
class TWSVelocityDataLink : public VelocityDataLink
{
public:
	TWSVelocityDataLink();

protected:
	virtual void regDatas();

public:
	int tm;    // 材质类型
};

class BridgeVelocityDataLink : public VelocityDataLink
{
public:
	BridgeVelocityDataLink();
};