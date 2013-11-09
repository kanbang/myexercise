#pragma once

#include "../GraphTool/BuildNetwork.h"

#include "EdgeData.h"
typedef Digraph::ArcMap<EdgeData*> SWD_EdgeDataMap;

// �����·�(����)���۷�(�ط�)
// ���ֽ�������ڷ�������ͼԪ��"ͨ������"������
class SetWindDir
{
public:
	SetWindDir(void);
	~SetWindDir(void);

	bool doSWD();

private:
	bool initNetwok();	                 // �������粢��ȡͼԪ��������
	void initAirEdges();                 // ��ʼ���÷�ص��֧
	bool isInitNetworkOk() const;        // ��ʼ�������Ƿ�ɹ�
	void setInitNetwokState(bool bState);// ���������ʼ��״̬(�ɹ�/ʧ��)

private:
	Digraph dg;                       // ͨ������(����ͼ)
	ArcFilter ef;                     // ��Ϸ�֧����
	Digraph::Node sn, tn;             // ��������Դ��ͻ��

	SWD_EdgeDataMap datas;            // ��֧������(��֧��ͨ�����ͣ����ֶ�)
	EdgeArray airEdges;               // �÷�ص��֧����
	bool bInitNetwokState;            // �����ʼ���Ƿ�ɹ�(Ĭ�ϳɹ�)
};
