#pragma once

#include "../MineGE/DataLink.h"

class AFCDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;                 // ����
	double q;                     // ����
	double userQ;                 // �û�ָ������
	
	/* ���ֶ����� */
	double maxQ;                  // ����������
};

// ������ģ��
class WS_AFCDataLink : public AFCDataLink
{
protected:
	virtual void regDatas();

public:
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

// ���������ģ��
class TWS_AFCDataLink : public AFCDataLink
{
protected:
	virtual void regDatas();

public:
	double area;                  // �������
	int tm;                       // ��������

	double q_ch4;                 // ƽ��������˹ӿ����
	double k_ch4;                 // ��˹ӿ��������ͨ��ϵ��

	int fanNum;                   // �ֲ�ͨ���̨��
	double q_fan;                 // �ֲ�ͨ���ʵ��������

	int maxPeopleNum;             // �������������
	double maxWeight;             // һ�α���ըҩ�������
};

// ��ը���Ͽ�ģ��
class Powder_AFCDataLink : public AFCDataLink
{
protected:
	virtual void regDatas();

public:
	double V;                     // ���
};

// �����ģ��
class Charge_AFCDataLink : public AFCDataLink
{
protected:
	virtual void regDatas();

public:
	double q_h2;                  // ������
};

// ��������ģ��
class Machine_AFCDataLink : public AFCDataLink
{
protected:
	virtual void regDatas();

public:
	double power;                // ����
	double theta;                // ����ϵ��
	double delta_t;              // �¶Ȳ�

	// �������ָ��һ���������������������һ������
	// ͨ���û�����Ҫ���û��������
};

enum AFC_NUM
{
	AN_1 = 0x0001,  // ���������˹ӿ��
	AN_2 = 0x0002,  // ��������
	AN_3 = 0x0004,  // ���ߵ糵��
	AN_4 = 0x0008   // ���ֳ�
};

// �������
class Tunnel_AFCDataLink : public AFCDataLink
{
public:
	Tunnel_AFCDataLink();
	~Tunnel_AFCDataLink();

protected:
	virtual void regDatas();

public:
	bool needAir;                 // �Ƿ񵥶����(���Ϊtrue,����Ҫ���м���)
	AFC_NUM an;                   // ������Щ�����ļ���

	double area;                  // �������
	int tm;                       // ��������
	double q_ch4;                 // ƽ��������˹ӿ����

	int ft;                       // ��������
	int num;                      // ̨��
	double power;                 // ����
};
