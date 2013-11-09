#pragma once

#include "../GraphTool/BuildNetwork.h"

#include "MVSSDataLink.h"
typedef AcArray<MVSSDataLink*> MVSSDataLinkArray;

/*
 * ˼·��
 *	    1) �������е�LinkedGEͼԪ����������������桢���ң����������������
 *		2) ��������ϵķ��źͷ細��������ڶ�����źͷ細��ֻ���ǵ�1������
 *		3) ��������ϵķ����������ڶ�������ֻ���ǵ�1�����
 *		   ע������ -- ���÷��Ż���ʱ����
 *             ��� --  ��Ҫͨ���
 *		4) �����ʶ��0��ʼ��ţ�����
 *			��� - 0, 1, ...
 *			�ڵ� - 0, 1, ...
 *			���� - 0, 1, ...
 *			��� - 0, 1, ...
 *		4) ��ͼԪ����ȡ���ݵ�MVSSDataLink��
 *		5) ����MVSSData�����ڴ������ݿ��д������
 * ���⣺
 *		1) �Ƿ���Ҫ���ǵ��ڷ細?
 *		2) �Ƿ���Ҫ���Ǿֲ��ȷ��?
 *		3) �Ƿ���Ҫ���Ǿ��������?
 *		4) �Ƿ���������ϴ��ڶ�����Ż������?
 *		5) ���������ڶ�����Ż���,��˼·���е�2)��3)����ʽ�Ƿ����?
 */
/* ͨ������ַ����(Ventilation Network Caculation),���VNC */
class MVSSVentNetworkCalc
{
public:
	MVSSVentNetworkCalc();
	~MVSSVentNetworkCalc();
	
	bool isInitNetworkOk() const;            // ��ʼ�������Ƿ�ɹ�

	// ������������ڵ����ݵķ���
	// �����Ҫ���ýڵ����ݣ���Ӧ��ִ��vnc֮ǰ����

	// ���ݵ������ȡ�ڵ�����
	JointDataLink* getJointData(const AcGePoint3d& pt);

	bool doVNC();                            // ִ��ͨ���������

private:
	bool initNetwok();
	void clear();
	void setInitNetwokState(bool bState);    // ���������ʼ��״̬(�ɹ�/ʧ��)	
	bool doMVSS_VNC(const CString& dbPath);  // ����MVSSģ������������

private:
	Digraph dg;                       // ͨ������(����ͼ)
	//Digraph::Node sn, tn;             // ��������Դ��ͻ��
	ObjectIdMap om;               // ��֧ͼԪid	
	NodeGeoDataMap ngm;           // �ڵ�����

	bool bInitNetwokState;
	MVSSDataLinkArray m_JDs;
	MVSSDataLinkArray m_TDs;
	MVSSDataLinkArray m_GDs;
	MVSSDataLinkArray m_FDs;
};
