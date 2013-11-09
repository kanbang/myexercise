#pragma once

#include "../GraphTool/BuildNetwork.h"

#include "EdgeData.h"
typedef Digraph::ArcMap<EdgeData*> SWD_EdgeDataMap;

// 划分新风(进风)和污风(回风)
// 划分结果保存在风流方向图元的"通风类型"数据上
class SetWindDir
{
public:
	SetWindDir(void);
	~SetWindDir(void);

	bool doSWD();

private:
	bool initNetwok();	                 // 构建网络并提取图元属性数据
	void initAirEdges();                 // 初始化用风地点分支
	bool isInitNetworkOk() const;        // 初始化网络是否成功
	void setInitNetwokState(bool bState);// 设置网络初始化状态(成功/失败)

private:
	Digraph dg;                       // 通风网络(有向图)
	ArcFilter ef;                     // 阻断分支过滤
	Digraph::Node sn, tn;             // 网络虚拟源点和汇点

	SWD_EdgeDataMap datas;            // 分支的数据(分支的通风类型，非字段)
	EdgeArray airEdges;               // 用风地点分支集合
	bool bInitNetwokState;            // 网络初始化是否成功(默认成功)
};
