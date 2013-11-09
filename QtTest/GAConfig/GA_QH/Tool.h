#pragma once

#include <ECF.h>
#include "GraphDef.h"

// 个体(染色体)转换成风阻值(VNR_EdgeMap中的风阻r)
extern void GenotypToR(Digraph& dg, EdgeArray& E, Binary::Binary* gen, VNR_EdgeMap& vnr_edge_map);

// 通风网络解算
extern bool VNR_VNC(Digraph& dg, VNR_EdgeMap& vnr_edge_map, VNR_NodeMap& vnr_node_map, VNR_Network_Data& vnr_network_data);
