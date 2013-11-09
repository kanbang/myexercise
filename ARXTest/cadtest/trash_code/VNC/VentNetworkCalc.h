#pragma once

#include "GraphHeader.h"

class IndependentCycle;
typedef AcArray<IndependentCycle*> CycleList;  // ������·����

// ͨ������ַ����
class VentNetworkCalc
{
public:
	VentNetworkCalc();
	~VentNetworkCalc();

	/*
	 * ��������ĳ�ʼ�ܷ���
	 */
	void setTotalQ(double q);

	/*
	 * ���õ�������(�������������)
	 * ������Ȳ���(0,1)֮��,��Ĭ��ΪDEF_PRECISE
	 * �μ�config.h
	 */
	void setPrecise(double precise);

	/* ���õ�������
	 * �����������С�ڵ���0,,��Ĭ��ΪDEF_MAX_COUNT
	 * �μ�config.h
	 */
	void setIterateCount(int maxCount);

	/*
	 * ��������
	 */
	bool iterate();

private:
	bool initNetwok();	                                // ��cadͼ�ζ�ȡ��������, 
	                                                    // �������粢��ȡͼԪ��������
	
	bool isInitNetworkOk() const;                       // ��ʼ�������Ƿ�ɹ�
	void setInitNetwokState(bool bState);               // ���������ʼ��״̬(�ɹ�/ʧ��)

	void addAirEdge();                                  // �������������֧
	Digraph::Arc getAirEdge() const;                    // �������������֧

	void findMST(EdgeArray& tree);                      // ������С������
	void findCycle(CycleList& cl);                      // ���Ҷ�����·
	void findCycleByTree(EdgeArray& tree, CycleList& cl);// ������С���������Ҷ�����·

	double sumQ(CycleList& cl, Digraph::Arc e);         // �����֧�����л�·�еķ���������
	void initQ(CycleList& cl, double Q);                // ��ʼ�����з�֧����(��֦+��֦)
	void initLeftTreeQ(CycleList& cl, double Q);        // ��ʼ����֦�ķ���
	void initLeftTreeQ1(EdgeArray& leftTree, double Q); // ��ʼ����֦�ķ���(������֧����֦)
	void initTreeQ(CycleList& cl);                      // ������֦������������֦�ķ���

	void initIterateVar(bool speedUp);            // ��ʼ����������
	bool doIterate(int count);                           // ����count��
	
private:
	Digraph dg;                       // ͨ������(����ͼ)
	ArcFilter ef;                     // ��Ϸ�֧����
	Digraph::Node sn, tn;             // ��������Դ��ͻ��

	NetworkEdgeDataMap datas;          // ��֧������(����, ����)
	bool bInitNetwokState;             // �����ʼ���Ƿ�ɹ�(Ĭ�ϳɹ�)
	Digraph::Arc airEdge;              // ���������֧(ֻ��һ����ֻ�����ʼ�������������������·��������)

	// ��������Ĳ���
	double totalQ;                     // �ܷ���
	double precise;                    // ����(0,1) -- Ĭ��Ϊ0.001
	int maxCount;                      // ��������
};
