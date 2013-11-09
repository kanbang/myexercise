#pragma once

//#include "dlimexp.h"
#define GRAPH_EXPORT_API 

#include "GraphDef.h"

/*
 *	创建网络
 *  同时额外处理掘进工作面(增加虚拟分支)
 */
extern "C" GRAPH_EXPORT_API bool BuildNetwork(
	const char* topology_data_filename,
	const char* vnr_data_filename,
	Digraph& dg, 
	VNR_EdgeMap& vnr_edge_map, 
	VNR_NodeMap& vnr_node_map,
	VNR_Network_Data& vnr_network_data);

/*
 * 处理多源多汇
 * 添加一个虚拟源点和虚拟汇点
 * 在虚拟源点和源点之间添加虚拟分支
 * 汇点也是同样处理
 */
extern "C" GRAPH_EXPORT_API bool AddVirtualSTNode(Digraph& dg, Digraph::Node& sn, Digraph::Node& tn);
