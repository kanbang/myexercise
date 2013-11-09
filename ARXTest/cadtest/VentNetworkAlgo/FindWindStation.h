#pragma once

#include "../GraphTool/BuildNetwork.h"

#include "EdgeData.h"
typedef Digraph::ArcMap<EdgeData*> FWS_EdgeDataMap;

// 最大路径长度
#define MAX_PATH_LENGTH 5

// 查找测风站
class FindWindStation
{
public:
	FindWindStation(void);
	~FindWindStation(void);

	Digraph::Arc getEdgeById(const AcDbObjectId& objId);       // 根据id查找分支
	AcDbObjectId getIdByEdge(Digraph::Arc e);                  // 根据分支查找id

	// 指定一条分支，在该分支的流入和流出的路径中查找有测风站的分支
	Digraph::Arc doFWS(Digraph::Arc e);

private:
	bool initNetwok();	                 // 构建网络并提取图元属性数据
	void initAirEdges();                 // 初始化用风地点分支
	bool isInitNetworkOk() const;        // 初始化网络是否成功
	void setInitNetwokState(bool bState);// 设置网络初始化状态(成功/失败)

private:
	Digraph dg;                       // 通风网络(有向图)
	ArcFilter ef;                     // 阻断分支过滤
	Digraph::Node sn, tn;             // 网络虚拟源点和汇点

	FWS_EdgeDataMap datas;            // 分支的数据(风量)
	EdgeArray airEdges;               // 用风地点分支集合
	bool bInitNetwokState;            // 网络初始化是否成功(默认成功)
};
