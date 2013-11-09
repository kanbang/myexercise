#pragma once

#include "../MineGE/DataLink.h"

// ͼԪ����
enum VD_TYPE
{
	VD_TUNNEL = 1, // ���
	VD_WS     = 2, // �زɹ�����
	VD_TWS    = 3, // ���������
	VD_BRIDGE = 4 // ���� -- �������ܷϳ�
	//VD_ROOM   = 5  // ��������
};

// ����ģ��--ú����101��
// ͼԪͨ�����ݣ����ơ����١���˹ӿ����
class VelocityDataLink : public DataLink
{
protected:
	virtual void regDatas();

protected:
	VelocityDataLink();

public:
	VD_TYPE type;         // ͼԪ����
	CString name;         // ����
	double v;             // ����
	double q_ch4;         // ƽ��������˹ӿ����
};

class TunnelVelocityDataLink : public VelocityDataLink
{
public:
	TunnelVelocityDataLink();

protected:
	virtual void regDatas();

public:
	int ft;               // �����������
	int sp;               // ��Ͳ��������
	bool isShaft;         // �Ƿ�Ͳ
};

// �زɹ�����ģ��
class WSVelocityDataLink : public VelocityDataLink
{
public:
	WSVelocityDataLink();

protected:
	virtual void regDatas();

public:
	int tm;    // ��������
	int cm;    // ��ú����
};

// ���������ģ��
class TWSVelocityDataLink : public VelocityDataLink
{
public:
	TWSVelocityDataLink();

protected:
	virtual void regDatas();

public:
	int tm;    // ��������
};

class BridgeVelocityDataLink : public VelocityDataLink
{
public:
	BridgeVelocityDataLink();
};