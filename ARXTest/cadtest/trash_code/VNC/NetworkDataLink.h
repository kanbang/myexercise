#pragma once

#include "../MineGE/DataLink.h"

// 分支类型
enum VNC_EdgeType
{
	VNC_COMMON   = 0,       // 一般分支(默认)
	VNC_FIXTED   = 1,       // 固定风量分支
	VNC_AIR      = 2        // 虚拟大气分支(网络中只有1条)
};

// 风机参数
// 目前将局扇和主扇的参数都看成是一样的
// 都使用5次多项式拟合
class FanDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	double fanPress_0(double q) const;  // 风机压力(0阶求导)
	double fanPress_1(double q) const;  // 风机压力(1阶求导)

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

// 分支参数
class EdgeDataLink : public DataLink
{
public:
	EdgeDataLink();
	~EdgeDataLink();

protected:
	virtual void regDatas();

public:
	// 一般分支
	void increase(double delta_q);     // 修正风量
	double edgeResistance_0() const;   // 求分支阻力(0阶求导)
	double edgeResistance_1() const;   // 求分支阻力(1阶求导)

	// 固定风量分支
	bool isFixedEdge() const;          // 是否固定风量分支

	// 风机分支
	bool isValidFanEdge() const;       // 是否有效的风机分支
	double fanPressure_0() const;      // 求风机分支的风机压力(0阶求导)
	double fanPressure_1() const;      // 求风机分支的风机压力(1阶求导)

	// 虚拟分支
	bool isVirtualAirEdge() const;     // 是否虚拟大气分支

	void initIterateVar(bool speedUp = false);      // 初始化迭代变量c的值

public:
	// 从图元中提取的数据
	CString name;     // 名称
	double r;         // 风阻
	double ar;        // 调节风阻
	double q;         // 风量
	double fq;        // 固定风量

	// 内部计算需要的自定义数据
	VNC_EdgeType et;    // 分支类型(默认一般分支)
	double c;           // 迭代计算的变量(初始等于r,用于迭代过程)

	// 目前尚未考虑自然风压

	FanDataLink* PFanData;  // 风机数据
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