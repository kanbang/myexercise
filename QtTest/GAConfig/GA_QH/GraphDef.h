#pragma once

#undef OUT
#undef IN

/* ʹ��lemon���ṩ�����ݽṹ����ͨ�õ�ͼ(������) */

/* ListDigraph��Arc��Node�ȱ�ʾͨ������ */
#include <lemon/list_graph.h>
using namespace lemon;

/* ����ͼ */
typedef ListDigraph Digraph;

#include <vector>
typedef std::vector<Digraph::Arc>    EdgeArray;
typedef std::vector<Digraph::Node>   NodeArray;

// ��֧����
struct VNR_Edge_Data
{
	int id;
	double r;     // ԭʼ����(�ⲿ����)
	double cr;    // ����(Ⱦɫ��)�����ķ���ֵ
	double q;     // ���Է���(�ⲿ����)
	double cq;    // �������������ķ���
	bool bInitQ;  // �Ƿ�����˷���

	// ����Ҫ���Ǹ�����֧��������???
	bool bInitR; // ��������
};

// �ڵ�����
struct VNR_Node_Data
{
	int id;
	double p;     // ����ѹ��(�ⲿ����)
	double cp;    // �������������Ľڵ�ѹ��
	bool bInitP;  // �Ƿ�����˽ڵ�ѹ��
};

// ��������
struct VNR_Network_Data
{
	double Q;   // �ܷ���
	double H;   // ������
	double CH;  // ����������������ѹ��
};

// ��֧����ӳ��
typedef Digraph::ArcMap<VNR_Edge_Data> VNR_EdgeMap;

// �ڵ�����ӳ��
typedef Digraph::NodeMap<VNR_Node_Data> VNR_NodeMap;