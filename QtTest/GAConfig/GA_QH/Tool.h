#pragma once

#include <ECF.h>
#include "GraphDef.h"

// ����(Ⱦɫ��)ת���ɷ���ֵ(VNR_EdgeMap�еķ���r)
extern void GenotypToR(Digraph& dg, EdgeArray& E, Binary::Binary* gen, VNR_EdgeMap& vnr_edge_map);

// ͨ���������
extern bool VNR_VNC(Digraph& dg, VNR_EdgeMap& vnr_edge_map, VNR_NodeMap& vnr_node_map, VNR_Network_Data& vnr_network_data);
