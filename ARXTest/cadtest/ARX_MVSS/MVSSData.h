#pragma once

#include "../DaoHelper/DaoData2.h"

#define JOINT_TABLE_NAME _T("�ڵ�")
#define TUNNEL_TABLE_NAME _T("���")
#define GATE_TABLE_NAME _T("������")
#define FAN_TABLE_NAME _T("ͨ�綯��")

#define INVALID_OBJ_ID -1

class MVSSDataLink;

// ��Ҫͬʱ�߱���д����
class MVSSData : public DaoData2
{
protected:
	MVSSData(const CString& tableName);

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);
	virtual void readFromTableImpl(DataReader* pDataReader);

public:
	int objId;  // ��ʶ
};

// �ڵ�����
class JointData : public MVSSData
{
public:
	JointData();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);
	virtual void readFromTableImpl(DataReader* pDataReader);

public:
	double x, y, z;   // ��������

	CString name;     // ����
	CString rp;       // ���ѹ��
	CString p;        // ����ѹ
	CString comment;  // ��ע
};

// �������
class TunnelData : public MVSSData
{
public:
	TunnelData();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);
	virtual void readFromTableImpl(DataReader* pDataReader);

public:
	int spt;          // ʼ�ڵ��ʶ
	int ept;          // ĩ�ڵ��ʶ

	CString name;     // ����
	CString q;        // ����
	CString r;        // Ħ������
	CString lr;       // �ֲ�����
	CString h;        // ������
	CString fq;       // �̶�����
	CString rho;      // �ܶ�
	CString ad;       // �ɵ�����
	CString ah;       // ��������
	CString ar;       // ���ڷ���
	CString rho1;     // �����ܶ�
	CString rho2;     // �ļ��ܶ�
	CString rho3;     // �＾�ܶ�
	CString rho4;     // �����ܶ�
	CString tq;       // ���Է���
	CString th;       // ��������
	CString l;        // �������
	CString apha;     // Ħ������ϵ��
	CString r100;     // ����Ħ������
	CString atype;    // ������״
	CString area;     // �������
	CString c;        // �ܳ�
	CString st;       // ֧����ʽ
	CString maxV;     // ����������
	CString tUse;     // �����;
	CString tRegion;  // ����
	CString temp;     // �¶�
	CString humidity; // ʪ��
	CString O2;       // ����Ũ��
	CString CH4;      // ����Ũ��
	CString CO2;      // ������̼Ũ��
	CString CO;       // һ����̼Ũ��
	CString reliability; // �ɿ���
	CString type;     // ����
	CString comment;  // ��ע
	CString eh;       // ��������
};

// ����������
class GateData : public MVSSData
{
public:
	GateData();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);
	virtual void readFromTableImpl(DataReader* pDataReader);

public:
	int tObjId;         // �����������ʶ
	double x, y, z;   // ��������

	CString name;     // ����
	CString	rq;       // ѹ��
	CString flow;     // ©����
	CString er;       // ��Ч����
	CString ea;       // ��Ч�細���
	CString trq;      // ����ѹ��
};

// �������
class FanData : public MVSSData
{
public:
	FanData();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);
	virtual void readFromTableImpl(DataReader* pDataReader);

public:
	int tObjId;         // �����������ʶ
	double x, y, z;   // ��������

	CString name;  // ����
	CString rs;    // ����״̬
	CString q;     // ����
	CString p;     // ��ѹ
	CString power; // ����
	CString efficiency; // Ч��
	CString r;     // ����
	CString aHole; // �Ȼ���
	CString model; // �ͺ�
	CString angle; // ҶƬ�Ƕ�
	CString	rotateSpeed; // ת��
	CString fq;    // �������
	CString nq;    // ��������
	CString lq;    // ©�����
	CString fsp;   // �����ѹ
	CString ffp;   // ���ȫѹ
	CString outVP; // ������ѹ
	CString inVP;  // �����ѹ
	CString inSP;  // ��ھ�ѹ
	CString tSP;   // ���Ծ�ѹ
	CString cPower;// �ĵ繦��
	CString inPower;// ���빦��
	CString outPower; // �������
	CString nPower;   // ��������
	CString lPower;   // ©�繦��
	CString uPower;   // ����Ч��
	CString ePower;   // ���Ч��
	CString sPower;   // ��ѹЧ��
	CString fPower;   // ȫѹЧ��
	CString fr;       // �������
	CString nr;       // ��������
	CString lr;       // ©�����
	CString fma;      // ���϶���
	CString fmv;      // ���Ϸ���
	CString outArea;  // ���ڶ���
	CString outV;     // ���ڷ���
	CString iq;       // ��ʼ����
	CString comment;  // ��ע
	CString	H_a0;
	CString H_a1;
	CString	H_a2;
	CString	H_a3;
	CString	H_a4;
	CString	H_a5;
	CString	N_a0;
	CString	N_a1;
	CString	N_a2;
	CString	N_a3;
	CString	N_a4;
	CString	N_a5;
	CString	E_a0;
	CString	E_a1;
	CString	E_a2;
	CString	E_a3;
	CString	E_a4;
	CString	E_a5;
};