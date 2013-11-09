#pragma once

#include "GraphData.h"

#include <vector>
typedef std::vector<Digraph::Arc> EdgeArray;
typedef std::vector<Digraph::Node> NodeArray;

// ������·ǰ������
class IndependentCycle;
typedef std::vector<IndependentCycle*> CycleList;

// ͨ������ַ����
class VentNetworkCalc
{
public:
	VentNetworkCalc(Digraph& dg, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd);
	~VentNetworkCalc();

	void setTotalQ(double totalQ);
	void setPrecise(double precise);
	void setIterateCount(int maxCount);
	bool iterate();                                     // ��������

private:
	Digraph::Arc getAirEdge() const;                    // �������������֧

	void initFixQEdges();                               // ��ʼ���̶�������֧
	void findMST(EdgeArray& tree);                      // ������С������
	void findCycle(CycleList& cl);                      // ���Ҷ�����·
	void findCycleByTree(EdgeArray& tree, CycleList& cl);// ������С���������Ҷ�����·

	double sumQ(CycleList& cl, Digraph::Arc e);         // �����֧�����л�·�еķ���������
	void initQ(CycleList& cl, double Q);                // ��ʼ�����з�֧����(��֦+��֦)
	void initLeftTreeQ(CycleList& cl, double Q);        // ��ʼ����֦�ķ���
	void initLeftTreeQ1(EdgeArray& leftTree, double Q); // ��ʼ����֦�ķ���(������֧����֦)
	void initTreeQ(CycleList& cl);                      // ������֦������������֦�ķ���

	bool doIterate(int count);                          // ����count��
	
public:
	Digraph& dg;                       // ͨ������(����ͼ)
	VNC_EdgeDataMap& ed;              // ��֧����
	VNC_FanDataMap& fd;               // ��֧�ϵķ������
	
	Digraph::Node sn, tn;              // ��������Դ��ͻ��
	Digraph::Arc airEdge;              // ���������֧(���->Դ��)
	bool speedUp;                      // �Ƿ���ü��ٵ�������

	double totalQ;                     // �ܷ���
	double precise;                    // ��������
	int maxCount;                      // ��������
};

extern double TotalH( Digraph& dg, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd );
