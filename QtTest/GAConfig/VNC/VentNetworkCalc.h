#pragma once

#include "GraphData.h"

#include <vector>
typedef std::vector<Digraph::Arc> EdgeArray;
typedef std::vector<Digraph::Node> NodeArray;

// 独立回路前向声明
class IndependentCycle;
typedef std::vector<IndependentCycle*> CycleList;

// 通风网络分风解算
class VentNetworkCalc
{
public:
	VentNetworkCalc(Digraph& dg, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd);
	~VentNetworkCalc();

	void setTotalQ(double totalQ);
	void setPrecise(double precise);
	void setIterateCount(int maxCount);
	bool iterate();                                     // 迭代计算

private:
	Digraph::Arc getAirEdge() const;                    // 返回虚拟大气分支

	void initFixQEdges();                               // 初始化固定风量分支
	void findMST(EdgeArray& tree);                      // 查找最小生成树
	void findCycle(CycleList& cl);                      // 查找独立回路
	void findCycleByTree(EdgeArray& tree, CycleList& cl);// 根据最小生成树查找独立回路

	double sumQ(CycleList& cl, Digraph::Arc e);         // 计算分支在所有回路中的风量代数和
	void initQ(CycleList& cl, double Q);                // 初始化所有分支风量(树枝+余枝)
	void initLeftTreeQ(CycleList& cl, double Q);        // 初始化余枝的风量
	void initLeftTreeQ1(EdgeArray& leftTree, double Q); // 初始化余枝的风量(大气分支是余枝)
	void initTreeQ(CycleList& cl);                      // 根据余枝风量，计算树枝的风量

	bool doIterate(int count);                          // 迭代count次
	
public:
	Digraph& dg;                       // 通风网络(有向图)
	VNC_EdgeDataMap& ed;              // 分支数据
	VNC_FanDataMap& fd;               // 分支上的风机数据
	
	Digraph::Node sn, tn;              // 网络虚拟源点和汇点
	Digraph::Arc airEdge;              // 虚拟大气分支(汇点->源点)
	bool speedUp;                      // 是否采用加速迭代方法

	double totalQ;                     // 总风量
	double precise;                    // 迭代精度
	int maxCount;                      // 迭代次数
};

extern double TotalH( Digraph& dg, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd );
