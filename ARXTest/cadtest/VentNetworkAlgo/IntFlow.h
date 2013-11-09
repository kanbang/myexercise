#pragma once

#include "../GraphTool/BuildNetwork.h"

#include "EdgeData.h"

// ��¼��֧������ֵ
struct ArcIntFlowData : public EdgeData
{
	ArcIntFlowData() : EdgeData(), iv(1) {}	
	int iv;           // Ĭ�ϵ���1
};

typedef Digraph::ArcMap<ArcIntFlowData*> ArcIntFlowMap;

// ������
class IntFlow
{
public:
	IntFlow(void);
	~IntFlow(void);

	Digraph::Arc getEdgeById(const AcDbObjectId& objId);       // ����id���ҷ�֧
	AcDbObjectId getIdByEdge(Digraph::Arc e);                  // ���ݷ�֧����id
	void doCacl();  // ִ�м�����̣�ÿ����֧��Ҫ����һ��intֵ

	// ������
	void displayIntValue();

private:
	bool initNetwok();	                 // �������粢��ȡͼԪ��������
	void initAirEdges();                 // ��ʼ���÷�ص��֧
	bool isInitNetworkOk() const;        // ��ʼ�������Ƿ�ɹ�
	void setInitNetwokState(bool bState);// ���������ʼ��״̬(�ɹ�/ʧ��)

	void initEdgeFlow();
	int caclEdgeOutFlow(Digraph::Arc e);
	int caclEdgeInFlow(Digraph::Arc e);

private:
	Digraph dg;                       // ͨ������(����ͼ)
	ArcFilter ef;                     // ��Ϸ�֧����
	Digraph::Node sn, tn;             // ��������Դ��ͻ��

	ArcIntFlowMap datas;                 // ��֧������(����)
	EdgeArray airEdges;                 // �÷�ص��֧����
	bool bInitNetwokState;            // �����ʼ���Ƿ�ɹ�(Ĭ�ϳɹ�)
};
