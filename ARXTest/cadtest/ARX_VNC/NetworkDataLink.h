#pragma once

#include "../MineGE/DataLink.h"

// ��֧����
class EdgeDataLink : public DataLink
{
public:
	EdgeDataLink();

protected:
	virtual void regDatas();

public:
	// ��ͼԪ����ȡ������
	CString name;     // ����
	double r;         // ����
	double ar;        // ���ڷ���
	double q;         // ����
	double fq;        // �̶�����
};

// �������
class FanDataLink : public DataLink
{
public:
	FanDataLink();

protected:
	virtual void regDatas();

public:
	// ��ͼԪ����ȡ������
	CString name;
	double a0;
	double a1;
	double a2;
	double a3;
	double a4;
	double a5;

	// ���ܻ���Ҫ��ȡ�����״̬(�������ػ�����ת)
};

// ���������(���š��細)
class GateDataLink : public DataLink
{
public:
	GateDataLink();

protected:
	virtual void regDatas();

public:
	double r;  // ��Ч����
};

// ��Ͳ����
class ChimneyDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	double r;         // ����
};

// ���з��š��細�Ĳ���
// Ŀǰ��ʱ��û�п���