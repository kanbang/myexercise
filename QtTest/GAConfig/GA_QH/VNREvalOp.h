#pragma once

#include "GraphDef.h"

// 通风网络阻力系数评价算子
class VNREvalOp : public EvaluateOp
{
public:
	VNREvalOp(Digraph& dg, EdgeArray& E, 
		VNR_Network_Data& vnr_network_data, 
		VNR_EdgeMap& vnr_edge_map, 
		VNR_NodeMap& vnr_node_map);

	// 计算适应值
	FitnessP evaluate(IndividualP individual);

private:
	Digraph& dg;                        // 通风网络
	EdgeArray& E;                       // 分支的排列顺序(与编码串的顺序一一对应)
	VNR_EdgeMap& vnr_edge_map;          // 分支数据(已测分支的风量、风阻)
 	VNR_NodeMap& vnr_node_map;          // 节点数据(已测节点的压力)
	VNR_Network_Data& vnr_network_data; // 总风量和总压力
};

typedef boost::shared_ptr<VNREvalOp> VNREvalOpP;
