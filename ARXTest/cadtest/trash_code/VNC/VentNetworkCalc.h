#pragma once

#include "GraphHeader.h"

class IndependentCycle;
typedef AcArray<IndependentCycle*> CycleList;  // 独立回路集合

// 通风网络分风解算
class VentNetworkCalc
{
public:
	VentNetworkCalc();
	~VentNetworkCalc();

	/*
	 * 设置网络的初始总风量
	 */
	void setTotalQ(double q);

	/*
	 * 设置迭代参数(精度与迭代次数)
	 * 如果精度不在(0,1)之间,则默认为DEF_PRECISE
	 * 参见config.h
	 */
	void setPrecise(double precise);

	/* 设置迭代次数
	 * 如果迭代次数小于等于0,,则默认为DEF_MAX_COUNT
	 * 参见config.h
	 */
	void setIterateCount(int maxCount);

	/*
	 * 迭代计算
	 */
	bool iterate();

private:
	bool initNetwok();	                                // 从cad图形读取几何数据, 
	                                                    // 构建网络并提取图元属性数据
	
	bool isInitNetworkOk() const;                       // 初始化网络是否成功
	void setInitNetwokState(bool bState);               // 设置网络初始化状态(成功/失败)

	void addAirEdge();                                  // 增加虚拟大气分支
	Digraph::Arc getAirEdge() const;                    // 返回虚拟大气分支

	void findMST(EdgeArray& tree);                      // 查找最小生成树
	void findCycle(CycleList& cl);                      // 查找独立回路
	void findCycleByTree(EdgeArray& tree, CycleList& cl);// 根据最小生成树查找独立回路

	double sumQ(CycleList& cl, Digraph::Arc e);         // 计算分支在所有回路中的风量代数和
	void initQ(CycleList& cl, double Q);                // 初始化所有分支风量(树枝+余枝)
	void initLeftTreeQ(CycleList& cl, double Q);        // 初始化余枝的风量
	void initLeftTreeQ1(EdgeArray& leftTree, double Q); // 初始化余枝的风量(大气分支是余枝)
	void initTreeQ(CycleList& cl);                      // 根据余枝风量，计算树枝的风量

	void initIterateVar(bool speedUp);            // 初始化迭代变量
	bool doIterate(int count);                           // 迭代count次
	
private:
	Digraph dg;                       // 通风网络(有向图)
	ArcFilter ef;                     // 阻断分支过滤
	Digraph::Node sn, tn;             // 网络虚拟源点和汇点

	NetworkEdgeDataMap datas;          // 分支的数据(风阻, 风量)
	bool bInitNetwokState;             // 网络初始化是否成功(默认成功)
	Digraph::Arc airEdge;              // 大气虚拟分支(只有一个，只参与初始化风量工作，不参与回路迭代计算)

	// 迭代计算的参数
	double totalQ;                     // 总风量
	double precise;                    // 精度(0,1) -- 默认为0.001
	int maxCount;                      // 迭代次数
};
