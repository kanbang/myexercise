#pragma once

#include "../GraphTool/BuildNetwork.h"

#include "EdgeData.h"
typedef Digraph::ArcMap<EdgeData*> IDS_EdgeDataMap;

// ʶ������ṹ
// ʵ�ֲ���ȫ???
class IdentifyDiagonalStructure
{
public:
	IdentifyDiagonalStructure(void);
	~IdentifyDiagonalStructure(void);

	Digraph::Arc getEdgeById(const AcDbObjectId& objId);       // ����id���ҷ�֧
	AcDbObjectId getIdByEdge(Digraph::Arc e);                  // ���ݷ�֧����id

	bool isDS(const AcDbObjectId& objId);
	bool doIDS(const AcDbObjectId& objId, AcDbObjectIdArray& p1, AcDbObjectIdArray& p2, bool findNST = false);

private:
	bool initNetwok();	                 // �������粢��ȡͼԪ��������
	bool isInitNetworkOk() const;        // ��ʼ�������Ƿ�ɹ�
	void setInitNetwokState(bool bState);// ���������ʼ��״̬(�ɹ�/ʧ��)
	void initAirEdges();                 // ��ʼ���÷�ص��֧

private:
	Digraph dg;                       // ͨ������(����ͼ)
	ArcFilter ef;                     // ��Ϸ�֧����
	Digraph::Node sn, tn;             // ��������Դ��ͻ��
	NodeGeoDataMap ngm;

	IDS_EdgeDataMap datas;         // ��֧������(��֧��ͨ�����ͣ����ֶ�)
	EdgeArray airEdges;                 // �÷�ص��֧����
	bool bInitNetwokState;            // �����ʼ���Ƿ�ɹ�(Ĭ�ϳɹ�)
};
