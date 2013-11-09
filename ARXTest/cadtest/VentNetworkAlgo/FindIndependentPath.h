#pragma once

#include "../GraphTool/BuildNetwork.h"

#include "EdgeData.h"
typedef Digraph::ArcMap<EdgeData*> FIP_EdgeDataMap;

// 搜索独立通路
class FindIndependentPath
{
public:
	FindIndependentPath(void);
	~FindIndependentPath(void);

	Digraph::Arc getEdgeById(const AcDbObjectId& objId);       // 根据id查找分支
	AcDbObjectId getIdByEdge(Digraph::Arc e);                  // 根据分支查找id

	bool doFip(Digraph::Arc e, EdgeArray& p1, EdgeArray& p2);      // 查找经过分支e的独立路线(进风路线p1和回风路线p2)
	void findAirEdges(AcDbObjectIdArray& objIds);              // 查找所有用风地点分支
	bool isIncludeAirEdge(EdgeArray& p);                         // 是否包含串联分支(用风地点)

	// 测试用
	void findSerialEdges(EdgeArray& p, EdgeArray& se);             // 在路径p中查找串联分支
	void displayPath(EdgeArray& p, unsigned short colorIndex);   // 用某种颜色显示路径

private:
	bool initNetwok();	                 // 构建网络并提取图元属性数据
	void initAirEdges();                 // 初始化用风地点分支
	bool isInitNetworkOk() const;        // 初始化网络是否成功
	void setInitNetwokState(bool bState);// 设置网络初始化状态(成功/失败)

private:
	Digraph dg;                       // 通风网络(有向图)
	ArcFilter ef;                     // 阻断分支过滤
	Digraph::Node sn, tn;             // 网络虚拟源点和汇点

	FIP_EdgeDataMap datas;            // 分支的数据(风量)
	EdgeArray airEdges;               // 用风地点分支集合
	bool bInitNetwokState;            // 网络初始化是否成功(默认成功)
};
