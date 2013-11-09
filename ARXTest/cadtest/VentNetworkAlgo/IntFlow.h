#pragma once

#include "../GraphTool/BuildNetwork.h"

#include "EdgeData.h"

// 记录分支的整数值
struct ArcIntFlowData : public EdgeData
{
	ArcIntFlowData() : EdgeData(), iv(1) {}	
	int iv;           // 默认等于1
};

typedef Digraph::ArcMap<ArcIntFlowData*> ArcIntFlowMap;

// 整数流
class IntFlow
{
public:
	IntFlow(void);
	~IntFlow(void);

	Digraph::Arc getEdgeById(const AcDbObjectId& objId);       // 根据id查找分支
	AcDbObjectId getIdByEdge(Digraph::Arc e);                  // 根据分支查找id
	void doCacl();  // 执行计算过程，每条分支都要分配一个int值

	// 测试用
	void displayIntValue();

private:
	bool initNetwok();	                 // 构建网络并提取图元属性数据
	void initAirEdges();                 // 初始化用风地点分支
	bool isInitNetworkOk() const;        // 初始化网络是否成功
	void setInitNetwokState(bool bState);// 设置网络初始化状态(成功/失败)

	void initEdgeFlow();
	int caclEdgeOutFlow(Digraph::Arc e);
	int caclEdgeInFlow(Digraph::Arc e);

private:
	Digraph dg;                       // 通风网络(有向图)
	ArcFilter ef;                     // 阻断分支过滤
	Digraph::Node sn, tn;             // 网络虚拟源点和汇点

	ArcIntFlowMap datas;                 // 分支的数据(风量)
	EdgeArray airEdges;                 // 用风地点分支集合
	bool bInitNetwokState;            // 网络初始化是否成功(默认成功)
};
