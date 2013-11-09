#pragma once

#include "../MineGE/DataLink.h"

// 第105条评价使用
// 应测风地点的数据
class StationTunnelDataLink : public DataLink
{
public:
	StationTunnelDataLink();

protected:
	virtual void regDatas();

public:
	CString name;        // 应测风地点名称
	bool hasWindStation;
	bool hasDone;
};

// 测风站图元数据
class StationDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	bool hasDone;  // 是否按规定测风
};
