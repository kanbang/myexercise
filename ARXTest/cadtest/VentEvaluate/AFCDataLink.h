#pragma once

#include "../MineGE/DataLink.h"

// ������ģ��
class WS_AFCDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;                 // ����
	double area;                  // �������
	double max_kdj, min_kdj;	  // ���ض������С�ض���
	double height;                // ʵ�ʲɸ�
	double reasonalVelocity;      // ���˷���
	double k_height;              // �ɸߵ���ϵ��
	double k_length;              // ���ȵ���ϵ��
	double k_temperature;         // �¶ȵ���ϵ��

	double q_ch4;                 // ƽ��������˹ӿ����
	double k_ch4;                 // ��˹ӿ��������ͨ��ϵ��
	double q_ch4_tail;            // ר����˹����˹ӿ����

	int maxPeopleNum;             // �������������
	double maxWeight;             // һ�α���ըҩ�������
};

// ������ģ��
class TWS_AFCDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;                 // ����
	double area;                  // �������
	int tunnelMaterial;           // ��������

	double q_ch4;                 // ƽ��������˹ӿ����
	double k_ch4;                 // ��˹ӿ��������ͨ��ϵ��

	int fanNum;                   // �ֲ�ͨ���̨��
	double q_fan;                 // �ֲ�ͨ���ʵ��������

	int maxPeopleNum;             // �������������
	double maxWeight;             // һ�α���ըҩ�������
};

// ��ը���Ͽ�ģ��
class Powder_AFCDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;                 // ����
	double V;                     // ���
};

// �����ģ��
class Charge_AFCDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;                 // ����
	double q_h2;                  // ������
};

// ��������ģ��
class Machine_AFCDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;                // ����
	
	double power;                // ����
	double theta;                // ����ϵ��
	double delta_t;              // �¶Ȳ�
};

enum AFC_NUM
{
	AN_1 = 0x0001,  // ���������˹ӿ��
	AN_2 = 0x0002,  // ��������
	AN_3 = 0x0004,  // ���ߵ糵��
};

// ���ֳ�ģ��
class RTC_AFCDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	int num;                      // ̨��
	double power;                 // ����
};

// �������
class Tunnel_AFCDataLink : public DataLink
{
public:
	Tunnel_AFCDataLink();
	~Tunnel_AFCDataLink();

protected:
	virtual void regDatas();

public:
	bool needAir;                 //�Ƿ񵥶����(���Ϊtrue,����Ҫ���м���)
	AFC_NUM an;                   // ������Щ�����ļ���

	CString name;                 // ����
	double area;                  // �������
	int tunnelMaterial;           // ��������
	double q_ch4;                 // ƽ��������˹ӿ����
	
	RTC_AFCDataLink* pRTCModel;
};
