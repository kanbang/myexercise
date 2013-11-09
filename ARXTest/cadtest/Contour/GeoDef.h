#pragma once

#include <vector>

// ��
struct DT_Point
{
	double x, y, z;
};

// ��
struct DT_Edge
{
	int s, t;
};

// ������
struct DT_Triangle
{
	int p1, p2, p3; // 3����
};

// ��ֵ��
struct DT_Contour
{
	int zi;                  // ��Ӧ��zֵ����
	int cnpts_num;           // ��ֵ�ߵ����
	int dir;                 // ��ʾ�պϵ�ֵ�߷���(zֵ����->�ڱ�Сdir=1, ��dir=-1), ���ŵ�ֵ��dir=0
};

// ����ֵ����߽�պϵ�ʱ��, ��չ��ֵ����Ϣ
struct DT_ContourExt
{
	int bpts_num;            // �߽�����
	int next_contour;        // ָ���������ĵ�ֵ��
};

// ��ֵ�߱�ע
struct DT_Label
{
	DT_Point pt;
	double angle;
};

// ���鶨��
typedef std::vector<DT_Point> PointArray;
typedef std::vector<DT_Edge> EdgeArray;
typedef std::vector<DT_Triangle> TriangleArray;
typedef std::vector<DT_Contour> ContourArray;
typedef std::vector<DT_ContourExt> ContourExtArray;
typedef std::vector<DT_Label> LabelArray;

// ��ʹ��
typedef std::vector<int> IntArray;
typedef std::vector<double> DoubleArray;