#pragma once

#include "../GraphTool/BuildNetwork.h"

#include "EdgeData.h"
typedef Digraph::ArcMap<EdgeData*> ILT_EdgeDataMap2;

// 进/回风之间的联络巷识别
// 以及判断是否安装有挡风墙或阻断风门(双向风门或平衡风门)
// 算法还存在一些不合理的地方
// 算法前提：无单向回路
class IdentifyLiaisonTunnel
{
public:
	IdentifyLiaisonTunnel(void);
	~IdentifyLiaisonTunnel(void);

	Digraph::Arc getEdgeById(const AcDbObjectId& objId);       // 根据id查找分支
	AcDbObjectId getIdByEdge(Digraph::Arc e);                  // 根据分支查找id

	bool doILT2(EdgeArray& edges);                                // 尝试查找所有进回风之间的联络巷
	bool doILT3(EdgeArray& edges);                                // 尝试查找所有主要的进回风联络巷
	bool doILT4(EdgeArray& edges);                                // (失败!!!)尝试查找进风或回风之间的联络巷
	bool findInOutEdges(Digraph::Arc e, EdgeArray& es1, EdgeArray& es2); // 查找联络巷的入边和出边分支集合
	
private:
	bool initNetwok();	                 // 构建网络并提取图元属性数据
	bool isInitNetworkOk() const;        // 初始化网络是否成功
	void setInitNetwokState(bool bState);// 设置网络初始化状态(成功/失败)
	void initAirEdges();                 // 初始化用风地点分支

private:
	Digraph dg;                       // 通风网络(有向图)
	ArcFilter ef;                     // 阻断分支过滤
	Digraph::Node sn, tn;             // 网络虚拟源点和汇点

	ILT_EdgeDataMap2 datas;         // 分支的数据(分支的通风类型，非字段)
	EdgeArray airEdges;                 // 用风地点分支集合
	bool bInitNetwokState;            // 网络初始化是否成功(默认成功)
};
