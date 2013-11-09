#pragma once

#include "dlimexp.h"

// �������
// Ŀǰ�����Ⱥ����ȵĲ�����������һ����
// ��ʹ��5�ζ���ʽ���
class VNC_EXPORT_API VNC_FanData
{
public:
	VNC_FanData();
	double h0(double q) const;  // ���ѹ��(0����)
	double h1(double q) const;  // ���ѹ��(1����)

public:
	double a0;  // (Ĭ�ϳ�ʼ��Ϊ0)
	double a1;  // (Ĭ�ϳ�ʼ��Ϊ0)
	double a2;  // (Ĭ�ϳ�ʼ��Ϊ0)
	double a3;  // (Ĭ�ϳ�ʼ��Ϊ0)
	double a4;  // (Ĭ�ϳ�ʼ��Ϊ0)
	double a5;  // (Ĭ�ϳ�ʼ��Ϊ0)
};

// ��֧����
class VNC_EXPORT_API VNC_EdgeData
{
public:
	VNC_EdgeData();
	double f0() const;    // ���֧����(0����)
	double f1() const;    // ���֧����(1����)

public:
	double r;         // ����(Ĭ�ϳ�ʼ��Ϊ0)
	double ar;        // ���ڷ���(Ĭ�ϳ�ʼ��Ϊ0)
	double q;         // ����(Ĭ�ϳ�ʼ��Ϊ0)
	double fq;        // �̶�����(Ĭ�ϳ�ʼ��Ϊ0)
};
