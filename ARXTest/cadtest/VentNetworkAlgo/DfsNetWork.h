#pragma once

#include "../GraphTool/BuildNetwork.h"

#include "EdgeData.h"
typedef Digraph::ArcMap<EdgeData*> DFS_EdgeDataMap;

class DfsNetWork
{
public:
	DfsNetWork(void);
	~DfsNetWork(void);

	Digraph::Arc getEdgeById(const AcDbObjectId& objId);       // ����id���ҷ�֧
	AcDbObjectId getIdByEdge(Digraph::Arc e);                  // ���ݷ�֧����id

	void doDFS(Digraph::Arc se, Digraph::Arc te, EdgeArray& p);

private:
	bool initNetwok();	                 // �������粢��ȡͼԪ��������
	bool isInitNetworkOk() const;        // ��ʼ�������Ƿ�ɹ�
	void setInitNetwokState(bool bState);// ���������ʼ��״̬(�ɹ�/ʧ��)
	void initAirEdges();                 // ��ʼ���÷�ص��֧

private:
	Digraph dg;                       // ͨ������(����ͼ)
	ArcFilter ef;                     // ��Ϸ�֧����
	Digraph::Node sn, tn;             // ��������Դ��ͻ��
	//NodeGeoDataMap ngm;

	DFS_EdgeDataMap datas;         // ��֧������(��֧��ͨ�����ͣ����ֶ�)
	EdgeArray airEdges;                 // �÷�ص��֧����
	bool bInitNetwokState;            // �����ʼ���Ƿ�ɹ�(Ĭ�ϳɹ�)
};
