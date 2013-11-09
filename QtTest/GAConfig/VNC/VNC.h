#pragma once

#include "dlimexp.h"

#include "GraphData.h"

/*
* 通风网络解算(Ventilation Network Calculation)
*   通风网络参数:
*		dg    -- 通风网络(连通且无单向回路)
*		ed    -- 分支数据
*		fd    -- 分支上的风机数据
*	 注: 如果分支上没有风机，则fd[e]=0;
*
*	迭代计算参数:
*		totalQ   -- 初始总风量
*		precise  -- 迭代计算精度
*		maxCount -- 最大迭代次数
*	输出结果:
*		返回值，表示解算是否成功
*
*
*	使用示例:
*		Digraph dg;
*		VNC_EdgeDataMap ed(dg); // 图分支绑定分支数据
*		VNC_FanDataMap fd(dg);  // 图分支绑定风机数据
*
*		// 创建网络dg，并初始化数据(ed和fd)
*		// ...
*
*		VNC(dg, ed, fd);               // 调用VNC模块
*
*		ClearAllVNCDatas(dg, ed, fd);  // 清除内存
*/
extern "C" VNC_EXPORT_API 
bool VNC(Digraph& dg, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd,
		 double totalQ = 50, double precise = 0.0001, int maxCount = 1000);

/*
 * 计算总阻力(排除风机动力)
 * 前提：网络解算成功，网络是平衡的
 */ 
extern "C" VNC_EXPORT_API 
double VNC_H(Digraph& dg, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd);

extern "C" VNC_EXPORT_API 
void ClearAllVNCDatas(Digraph& dg, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd);