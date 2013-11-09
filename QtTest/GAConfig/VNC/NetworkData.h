#pragma once

#include "dlimexp.h"

// 风机数据
// 目前将局扇和主扇的参数都看成是一样的
// 都使用5次多项式拟合
class VNC_EXPORT_API VNC_FanData
{
public:
	VNC_FanData();
	double h0(double q) const;  // 风机压力(0阶求导)
	double h1(double q) const;  // 风机压力(1阶求导)

public:
	double a0;  // (默认初始化为0)
	double a1;  // (默认初始化为0)
	double a2;  // (默认初始化为0)
	double a3;  // (默认初始化为0)
	double a4;  // (默认初始化为0)
	double a5;  // (默认初始化为0)
};

// 分支数据
class VNC_EXPORT_API VNC_EdgeData
{
public:
	VNC_EdgeData();
	double f0() const;    // 求分支阻力(0阶求导)
	double f1() const;    // 求分支阻力(1阶求导)

public:
	double r;         // 风阻(默认初始化为0)
	double ar;        // 调节风阻(默认初始化为0)
	double q;         // 风量(默认初始化为0)
	double fq;        // 固定风量(默认初始化为0)
};
