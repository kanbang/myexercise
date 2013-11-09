#pragma once

#include "../GraphTool/BuildNetwork.h"

#include "MVSSDataLink.h"
typedef AcArray<MVSSDataLink*> MVSSDataLinkArray;

/*
 * 思路：
 *	    1) 查找所有的LinkedGE图元，包括巷道、工作面、硐室，不包括掘进工作面
 *		2) 查找巷道上的风门和风窗，如果存在多个风门和风窗，只考虑第1个风门
 *		3) 查找巷道上的风机，如果存在多个风机，只考虑第1个风机
 *		   注：风门 -- 永久风门或临时风门
 *             风机 --  主要通风机
 *		4) 对象标识从0开始编号，例如
 *			巷道 - 0, 1, ...
 *			节点 - 0, 1, ...
 *			风门 - 0, 1, ...
 *			风机 - 0, 1, ...
 *		4) 从图元中提取数据到MVSSDataLink中
 *		5) 创建MVSSData，用于创建数据库表及写入数据
 * 问题：
 *		1) 是否需要考虑调节风窗?
 *		2) 是否需要考虑局部扇风机?
 *		3) 是否需要考虑掘进工作面?
 *		4) 是否允许巷道上存在多个风门或多个风机?
 *		5) 如果允许存在多个风门或风机,【思路】中的2)和3)处理方式是否合理?
 */
/* 通风网络分风解算(Ventilation Network Caculation),简称VNC */
class MVSSVentNetworkCalc
{
public:
	MVSSVentNetworkCalc();
	~MVSSVentNetworkCalc();
	
	bool isInitNetworkOk() const;            // 初始化网络是否成功

	// 增加用于输入节点数据的方法
	// 如果需要设置节点数据，则应在执行vnc之前输入

	// 根据点坐标获取节点数据
	JointDataLink* getJointData(const AcGePoint3d& pt);

	bool doVNC();                            // 执行通风网络解算

private:
	bool initNetwok();
	void clear();
	void setInitNetwokState(bool bState);    // 设置网络初始化状态(成功/失败)	
	bool doMVSS_VNC(const CString& dbPath);  // 调用MVSS模块进行网络解算

private:
	Digraph dg;                       // 通风网络(有向图)
	//Digraph::Node sn, tn;             // 网络虚拟源点和汇点
	ObjectIdMap om;               // 分支图元id	
	NodeGeoDataMap ngm;           // 节点坐标

	bool bInitNetwokState;
	MVSSDataLinkArray m_JDs;
	MVSSDataLinkArray m_TDs;
	MVSSDataLinkArray m_GDs;
	MVSSDataLinkArray m_FDs;
};
