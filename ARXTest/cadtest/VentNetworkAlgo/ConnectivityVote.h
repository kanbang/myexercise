#pragma once

#include "../GraphTool/BuildNetwork.h"
typedef Digraph::ArcMap<AcDbObjectId> CNN_EdgeDataMap; // 只关联id

// 连通性判定
class ConnectivityVote
{
public:
	ConnectivityVote(void);
	~ConnectivityVote(void);

	bool connected();              // 判断网络是否连通
	bool dag();                    // 判断是否有向无环图

	/* 
	 * 查找连通块
	 * ccIds的长度表示连通块的个数，索引值代表连通块标识
	 * ccIds[i]表示第i个连通块的分支个数
	 * objIds存储所有的连通块的分支集合(可能存在分支重复)
	 * 第i个连通块对应的分支可以通过i与ccIds[i]计算得到区间范围
	 */
	bool CC(AcDbObjectIdArray& objIds, AcDbIntArray& ccIds);
	bool SCC(AcDbObjectIdArray& objIds, AcDbIntArray& sccIds);

	void FIE(AcDbObjectIdArray& objIds);                   // 查找进风井
	void FOE(AcDbObjectIdArray& objIds);                  // 查找回风井

private:
	bool initNetwok();	                 // 构建网络并提取图元属性数据
	bool isInitNetworkOk() const;        // 初始化网络是否成功
	void setInitNetwokState(bool bState);// 设置网络初始化状态(成功/失败)

private:
	Digraph dg;                       // 通风网络(有向图)
	ArcFilter ef;                     // 阻断分支过滤
	Digraph::Node sn, tn;             // 网络虚拟源点和汇点

	CNN_EdgeDataMap datas;            // 分支的属性数据
	bool bInitNetwokState;            // 网络初始化是否成功(默认成功)
};
