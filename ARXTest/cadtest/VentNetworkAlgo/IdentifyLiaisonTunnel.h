#pragma once

#include "../GraphTool/BuildNetwork.h"

#include "EdgeData.h"
typedef Digraph::ArcMap<EdgeData*> ILT_EdgeDataMap2;

// ��/�ط�֮���������ʶ��
// �Լ��ж��Ƿ�װ�е���ǽ����Ϸ���(˫����Ż�ƽ�����)
// �㷨������һЩ������ĵط�
// �㷨ǰ�᣺�޵����·
class IdentifyLiaisonTunnel
{
public:
	IdentifyLiaisonTunnel(void);
	~IdentifyLiaisonTunnel(void);

	Digraph::Arc getEdgeById(const AcDbObjectId& objId);       // ����id���ҷ�֧
	AcDbObjectId getIdByEdge(Digraph::Arc e);                  // ���ݷ�֧����id

	bool doILT2(EdgeArray& edges);                                // ���Բ������н��ط�֮���������
	bool doILT3(EdgeArray& edges);                                // ���Բ���������Ҫ�Ľ��ط�������
	bool doILT4(EdgeArray& edges);                                // (ʧ��!!!)���Բ��ҽ����ط�֮���������
	bool findInOutEdges(Digraph::Arc e, EdgeArray& es1, EdgeArray& es2); // �������������ߺͳ��߷�֧����
	
private:
	bool initNetwok();	                 // �������粢��ȡͼԪ��������
	bool isInitNetworkOk() const;        // ��ʼ�������Ƿ�ɹ�
	void setInitNetwokState(bool bState);// ���������ʼ��״̬(�ɹ�/ʧ��)
	void initAirEdges();                 // ��ʼ���÷�ص��֧

private:
	Digraph dg;                       // ͨ������(����ͼ)
	ArcFilter ef;                     // ��Ϸ�֧����
	Digraph::Node sn, tn;             // ��������Դ��ͻ��

	ILT_EdgeDataMap2 datas;         // ��֧������(��֧��ͨ�����ͣ����ֶ�)
	EdgeArray airEdges;                 // �÷�ص��֧����
	bool bInitNetwokState;            // �����ʼ���Ƿ�ɹ�(Ĭ�ϳɹ�)
};
