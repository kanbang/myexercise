#pragma once

#include "../GraphTool/BuildNetwork.h"

#include "EdgeData.h"
typedef Digraph::ArcMap<EdgeData*> FWS_EdgeDataMap;

// ���·������
#define MAX_PATH_LENGTH 5

// ���Ҳ��վ
class FindWindStation
{
public:
	FindWindStation(void);
	~FindWindStation(void);

	Digraph::Arc getEdgeById(const AcDbObjectId& objId);       // ����id���ҷ�֧
	AcDbObjectId getIdByEdge(Digraph::Arc e);                  // ���ݷ�֧����id

	// ָ��һ����֧���ڸ÷�֧�������������·���в����в��վ�ķ�֧
	Digraph::Arc doFWS(Digraph::Arc e);

private:
	bool initNetwok();	                 // �������粢��ȡͼԪ��������
	void initAirEdges();                 // ��ʼ���÷�ص��֧
	bool isInitNetworkOk() const;        // ��ʼ�������Ƿ�ɹ�
	void setInitNetwokState(bool bState);// ���������ʼ��״̬(�ɹ�/ʧ��)

private:
	Digraph dg;                       // ͨ������(����ͼ)
	ArcFilter ef;                     // ��Ϸ�֧����
	Digraph::Node sn, tn;             // ��������Դ��ͻ��

	FWS_EdgeDataMap datas;            // ��֧������(����)
	EdgeArray airEdges;               // �÷�ص��֧����
	bool bInitNetwokState;            // �����ʼ���Ƿ�ɹ�(Ĭ�ϳɹ�)
};
