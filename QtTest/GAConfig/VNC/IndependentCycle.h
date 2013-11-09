#pragma once

#include "GraphData.h"

#include <vector>
typedef std::vector<Digraph::Arc> EdgeArray;
typedef std::vector<short> EdgeDirCoeffArray;  // ��·�еķ�֧����ϵ��

// ������·
class IndependentCycle
{
public:
	IndependentCycle(Digraph& dg, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd, Digraph::Arc airEdge, Digraph::Arc baseEdge);

	Digraph::Arc getBaseEdge() const;              // ���ػ�׼��֧

	void addEdge(Digraph::Arc e);                  // ���·�����ӷ�֧(�ų���׼��֧)
	void getPath(EdgeArray& p);                    // ���ػ�·�е�������֧(1��·��)

	void init();                                   // ��ӷ�֧���֮�󣬽��г�ʼ��

	short getCycleCoeff(Digraph::Arc e);              // ���֧�ڻ�·�еķ���ϵ��(c={1, -1, 0})
	void iterate(double& delta_q, double& delta_f);   // ��������,�����ط���У��ֵ�ͻ�·����������	   

	// ������
	//void printMsg();
	//void printEdge(Digraph::Arc e);
	//void printQMsg(const CString& msg);

private:
	void initDirection();           // ��ʼ����·��֧����ϵ��
	bool canDoIterate();            // �жϻ�·�ܷ���е�������
	bool isFixQCycle();             // �Ƿ�̶�������·
	bool hasExtraF();               // ��·�Ƿ������������(�������)
	double deltaQ() const;          // �����·�ķ�������ֵ
	void increaseQ(Digraph::Arc e, double q); // ������֧����

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
	VNC_EdgeDataMap& ed;   // ��֧����
	VNC_FanDataMap& fd;    // �������
	Digraph::Arc airEdge;   // ���������֧

	Digraph::Arc baseEdge;       // ��׼��֧
	EdgeArray path;              // ��·��֧����(��������׼��֧)
	EdgeDirCoeffArray c;         // ��֧����ϵ��

	bool bCanDoIterate;          // ��·�Ƿ���Խ��е�������
};