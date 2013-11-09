#pragma once

#include "../MineGE/DataLink.h"

// 分支参数
class EdgeDataLink : public DataLink
{
public:
	EdgeDataLink();

protected:
	virtual void regDatas();

public:
	// 从图元中提取的数据
	CString name;     // 名称
	double r;         // 风阻
	double ar;        // 调节风阻
	double q;         // 风量
	double fq;        // 固定风量
};

// 风机参数
class FanDataLink : public DataLink
{
public:
	FanDataLink();

protected:
	virtual void regDatas();

public:
	// 从图元中提取的数据
	CString name;
	double a0;
	double a1;
	double a2;
	double a3;
	double a4;
	double a5;

	// 可能还需要提取风机的状态(开机、关机、翻转)
};

// 构筑物参数(风门、风窗)
class GateDataLink : public DataLink
{
public:
	GateDataLink();

protected:
	virtual void regDatas();

public:
	double r;  // 等效风阻
};

// 风筒参数
class ChimneyDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	double r;         // 风阻
};

// 还有风门、风窗的参数
// 目前暂时还没有考虑