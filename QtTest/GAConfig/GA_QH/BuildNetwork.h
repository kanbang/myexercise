#pragma once

//#include "dlimexp.h"
#define GRAPH_EXPORT_API 

#include "GraphDef.h"

/*
 *	��������
 *  ͬʱ���⴦����������(���������֧)
 */
extern "C" GRAPH_EXPORT_API bool BuildNetwork(
	const char* topology_data_filename,
	const char* vnr_data_filename,
	Digraph& dg, 
	VNR_EdgeMap& vnr_edge_map, 
	VNR_NodeMap& vnr_node_map,
	VNR_Network_Data& vnr_network_data);

/*
 * �����Դ���
 * ���һ������Դ���������
 * ������Դ���Դ��֮����������֧
 * ���Ҳ��ͬ������
 */
extern "C" GRAPH_EXPORT_API bool AddVirtualSTNode(Digraph& dg, Digraph::Node& sn, Digraph::Node& tn);
