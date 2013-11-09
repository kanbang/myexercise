#pragma once

#include "GraphDef.h"

// ͨ����������ϵ����������
class VNREvalOp : public EvaluateOp
{
public:
	VNREvalOp(Digraph& dg, EdgeArray& E, 
		VNR_Network_Data& vnr_network_data, 
		VNR_EdgeMap& vnr_edge_map, 
		VNR_NodeMap& vnr_node_map);

	// ������Ӧֵ
	FitnessP evaluate(IndividualP individual);

private:
	Digraph& dg;                        // ͨ������
	EdgeArray& E;                       // ��֧������˳��(����봮��˳��һһ��Ӧ)
	VNR_EdgeMap& vnr_edge_map;          // ��֧����(�Ѳ��֧�ķ���������)
 	VNR_NodeMap& vnr_node_map;          // �ڵ�����(�Ѳ�ڵ��ѹ��)
	VNR_Network_Data& vnr_network_data; // �ܷ�������ѹ��
};

typedef boost::shared_ptr<VNREvalOp> VNREvalOpP;
