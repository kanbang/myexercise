#pragma once

#include "../MineGE/DataLink.h"

// ��֧����
enum VNC_EdgeType
{
	VNC_COMMON   = 0,       // һ���֧(Ĭ��)
	VNC_FIXTED   = 1,       // �̶�������֧
	VNC_AIR      = 2        // ���������֧(������ֻ��1��)
};

// �������
// Ŀǰ�����Ⱥ����ȵĲ�����������һ����
// ��ʹ��5�ζ���ʽ���
class FanDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	double fanPress_0(double q) const;  // ���ѹ��(0����)
	double fanPress_1(double q) const;  // ���ѹ��(1����)

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

// ��֧����
class EdgeDataLink : public DataLink
{
public:
	EdgeDataLink();
	~EdgeDataLink();

protected:
	virtual void regDatas();

public:
	// һ���֧
	void increase(double delta_q);     // ��������
	double edgeResistance_0() const;   // ���֧����(0����)
	double edgeResistance_1() const;   // ���֧����(1����)

	// �̶�������֧
	bool isFixedEdge() const;          // �Ƿ�̶�������֧

	// �����֧
	bool isValidFanEdge() const;       // �Ƿ���Ч�ķ����֧
	double fanPressure_0() const;      // ������֧�ķ��ѹ��(0����)
	double fanPressure_1() const;      // ������֧�ķ��ѹ��(1����)

	// �����֧
	bool isVirtualAirEdge() const;     // �Ƿ����������֧

	void initIterateVar(bool speedUp = false);      // ��ʼ����������c��ֵ

public:
	// ��ͼԪ����ȡ������
	CString name;     // ����
	double r;         // ����
	double ar;        // ���ڷ���
	double q;         // ����
	double fq;        // �̶�����

	// �ڲ�������Ҫ���Զ�������
	VNC_EdgeType et;    // ��֧����(Ĭ��һ���֧)
	double c;           // ��������ı���(��ʼ����r,���ڵ�������)

	// Ŀǰ��δ������Ȼ��ѹ

	FanDataLink* PFanData;  // �������
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