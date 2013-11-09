#pragma once

#include "../GraphTool/BuildNetwork.h"
typedef Digraph::ArcMap<AcDbObjectId> CNN_EdgeDataMap; // ֻ����id

// ��ͨ���ж�
class ConnectivityVote
{
public:
	ConnectivityVote(void);
	~ConnectivityVote(void);

	bool connected();              // �ж������Ƿ���ͨ
	bool dag();                    // �ж��Ƿ������޻�ͼ

	/* 
	 * ������ͨ��
	 * ccIds�ĳ��ȱ�ʾ��ͨ��ĸ���������ֵ������ͨ���ʶ
	 * ccIds[i]��ʾ��i����ͨ��ķ�֧����
	 * objIds�洢���е���ͨ��ķ�֧����(���ܴ��ڷ�֧�ظ�)
	 * ��i����ͨ���Ӧ�ķ�֧����ͨ��i��ccIds[i]����õ����䷶Χ
	 */
	bool CC(AcDbObjectIdArray& objIds, AcDbIntArray& ccIds);
	bool SCC(AcDbObjectIdArray& objIds, AcDbIntArray& sccIds);

	void FIE(AcDbObjectIdArray& objIds);                   // ���ҽ��羮
	void FOE(AcDbObjectIdArray& objIds);                  // ���һط羮

private:
	bool initNetwok();	                 // �������粢��ȡͼԪ��������
	bool isInitNetworkOk() const;        // ��ʼ�������Ƿ�ɹ�
	void setInitNetwokState(bool bState);// ���������ʼ��״̬(�ɹ�/ʧ��)

private:
	Digraph dg;                       // ͨ������(����ͼ)
	ArcFilter ef;                     // ��Ϸ�֧����
	Digraph::Node sn, tn;             // ��������Դ��ͻ��

	CNN_EdgeDataMap datas;            // ��֧����������
	bool bInitNetwokState;            // �����ʼ���Ƿ�ɹ�(Ĭ�ϳɹ�)
};
