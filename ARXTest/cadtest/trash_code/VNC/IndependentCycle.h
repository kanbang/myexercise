#pragma once

#include "GraphHeader.h"

// 独立回路
class IndependentCycle
{
public:
	IndependentCycle(Digraph& dg, NetworkEdgeDataMap& datas, Digraph::Arc baseEdge);

	Digraph::Arc getBaseEdge() const;              // 返回基准分支

	void addEdge(Digraph::Arc e);                  // 向回路中增加分支(排除基准分支)
	void getPath(EdgeArray& p);                    // 返回回路中的其它分支(1条路径)

	void init();                                   // 添加分支完成之后，进行初始化

	short getCycleCoeff(Digraph::Arc e);              // 求分支在回路中的方向系数(c={1, -1, 0})
	void iterate(double& delta_q, double& delta_f);   // 迭代计算,并返回风量校正值和回路阻力代数和	   

	// 测试用
	void printMsg();
	void printEdge(Digraph::Arc e);
	void printQMsg(const CString& msg);

private:
	void initDirection();           // 初始化回路分支方向系数
	bool canDoIterate();            // 判断回路能否进行迭代计算
	bool isFixQCycle();             // 该回路是固定风量回路
	bool hasExtraF();               // 该回路中有附加阻力
	double deltaQ() const;          // 计算回路的风量修正值

	double f0() const;              // 回路阻力代数和(包含基准分支)-0阶求导
	double f1() const;              // 回路阻力代数和(包含基准分支)-1阶导数
	double h0() const;              // 计算风机压力的0阶求导(如果是风机分支)
	double h1() const;              // 计算风机压力的1阶求导(如果是风机分支)

	double f0(Digraph::Arc e) const;     // 求分支阻力(0阶求导)
	double f1(Digraph::Arc e) const;     // 求分支阻力(1阶求导)
	double h0(Digraph::Arc e) const;     // 求风机分支的风机压力(0阶求导)
	double h1(Digraph::Arc e) const;     // 求风机分支的风机压力(1阶求导)
	
private:
	Digraph& dg;
	NetworkEdgeDataMap& datas;   // 分支数据

	Digraph::Arc baseEdge;       // 基准分支
	EdgeArray path;              // 回路分支集合(不包含基准分支)
	EdgeDirCoeffArray c;         // 分支方向系数

	bool bCanDoIterate;          // 回路是否可以进行迭代计算
};