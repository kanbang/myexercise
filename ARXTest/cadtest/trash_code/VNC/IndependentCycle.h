#pragma once

#include "GraphHeader.h"

// ������·
class IndependentCycle
{
public:
	IndependentCycle(Digraph& dg, NetworkEdgeDataMap& datas, Digraph::Arc baseEdge);

	Digraph::Arc getBaseEdge() const;              // ���ػ�׼��֧

	void addEdge(Digraph::Arc e);                  // ���·�����ӷ�֧(�ų���׼��֧)
	void getPath(EdgeArray& p);                    // ���ػ�·�е�������֧(1��·��)

	void init();                                   // ��ӷ�֧���֮�󣬽��г�ʼ��

	short getCycleCoeff(Digraph::Arc e);              // ���֧�ڻ�·�еķ���ϵ��(c={1, -1, 0})
	void iterate(double& delta_q, double& delta_f);   // ��������,�����ط���У��ֵ�ͻ�·����������	   

	// ������
	void printMsg();
	void printEdge(Digraph::Arc e);
	void printQMsg(const CString& msg);

private:
	void initDirection();           // ��ʼ����·��֧����ϵ��
	bool canDoIterate();            // �жϻ�·�ܷ���е�������
	bool isFixQCycle();             // �û�·�ǹ̶�������·
	bool hasExtraF();               // �û�·���и�������
	double deltaQ() const;          // �����·�ķ�������ֵ

	double f0() const;              // ��·����������(������׼��֧)-0����
	double f1() const;              // ��·����������(������׼��֧)-1�׵���
	double h0() const;              // ������ѹ����0����(����Ƿ����֧)
	double h1() const;              // ������ѹ����1����(����Ƿ����֧)

	double f0(Digraph::Arc e) const;     // ���֧����(0����)
	double f1(Digraph::Arc e) const;     // ���֧����(1����)
	double h0(Digraph::Arc e) const;     // ������֧�ķ��ѹ��(0����)
	double h1(Digraph::Arc e) const;     // ������֧�ķ��ѹ��(1����)
	
private:
	Digraph& dg;
	NetworkEdgeDataMap& datas;   // ��֧����

	Digraph::Arc baseEdge;       // ��׼��֧
	EdgeArray path;              // ��·��֧����(��������׼��֧)
	EdgeDirCoeffArray c;         // ��֧����ϵ��

	bool bCanDoIterate;          // ��·�Ƿ���Խ��е�������
};