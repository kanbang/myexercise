#pragma once

#undef OUT
#undef IN

/* 使用lemon库提供的数据结构定义通用的图(或网络) */

/* ListDigraph、Arc、Node等表示通风网络 */
#include <lemon/list_graph.h>
using namespace lemon;

/* 有向图 */
typedef ListDigraph Digraph;

#include <vector>
typedef std::vector<Digraph::Arc>    EdgeArray;
typedef std::vector<Digraph::Node>   NodeArray;

// 分支数据
struct VNR_Edge_Data
{
	int id;
	double r;     // 原始风阻(外部输入)
	double cr;    // 个体(染色体)解码后的风阻值
	double q;     // 测试风量(外部输入)
	double cq;    // 网络解算后计算出的风量
	bool bInitQ;  // 是否测试了风量

	// 还需要考虑给定分支风阻的情况???
	bool bInitR; // 给定风阻
};

// 节点数据
struct VNR_Node_Data
{
	int id;
	double p;     // 测试压力(外部输入)
	double cp;    // 网络解算后计算出的节点压力
	bool bInitP;  // 是否测试了节点压力
};

// 网络数据
struct VNR_Network_Data
{
	double Q;   // 总风量
	double H;   // 总阻力
	double CH;  // 网络解算后计算出的总压力
};

// 分支数据映射
typedef Digraph::ArcMap<VNR_Edge_Data> VNR_EdgeMap;

// 节点数据映射
typedef Digraph::NodeMap<VNR_Node_Data> VNR_NodeMap;