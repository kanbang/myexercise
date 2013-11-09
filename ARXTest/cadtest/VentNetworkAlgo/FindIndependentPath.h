#pragma once

#include "../GraphTool/BuildNetwork.h"

#include "EdgeData.h"
typedef Digraph::ArcMap<EdgeData*> FIP_EdgeDataMap;

// ��������ͨ·
class FindIndependentPath
{
public:
	FindIndependentPath(void);
	~FindIndependentPath(void);

	Digraph::Arc getEdgeById(const AcDbObjectId& objId);       // ����id���ҷ�֧
	AcDbObjectId getIdByEdge(Digraph::Arc e);                  // ���ݷ�֧����id

	bool doFip(Digraph::Arc e, EdgeArray& p1, EdgeArray& p2);      // ���Ҿ�����֧e�Ķ���·��(����·��p1�ͻط�·��p2)
	void findAirEdges(AcDbObjectIdArray& objIds);              // ���������÷�ص��֧
	bool isIncludeAirEdge(EdgeArray& p);                         // �Ƿ����������֧(�÷�ص�)

	// ������
	void findSerialEdges(EdgeArray& p, EdgeArray& se);             // ��·��p�в��Ҵ�����֧
	void displayPath(EdgeArray& p, unsigned short colorIndex);   // ��ĳ����ɫ��ʾ·��

private:
	bool initNetwok();	                 // �������粢��ȡͼԪ��������
	void initAirEdges();                 // ��ʼ���÷�ص��֧
	bool isInitNetworkOk() const;        // ��ʼ�������Ƿ�ɹ�
	void setInitNetwokState(bool bState);// ���������ʼ��״̬(�ɹ�/ʧ��)

private:
	Digraph dg;                       // ͨ������(����ͼ)
	ArcFilter ef;                     // ��Ϸ�֧����
	Digraph::Node sn, tn;             // ��������Դ��ͻ��

	FIP_EdgeDataMap datas;            // ��֧������(����)
	EdgeArray airEdges;               // �÷�ص��֧����
	bool bInitNetwokState;            // �����ʼ���Ƿ�ɹ�(Ĭ�ϳɹ�)
};
