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
    int zi;                  // ��Ӧ��zֵ����(����Ǳ߽�, �����-1)
    int cnpts_num;           // ��ֵ�ߵ����(����Ǳ߽�,�����0)

    int dir;                 // �պϵ�ֵ�߷���(����->�ڱ�Сdir=1, ��dir=-1), ���ŵ�ֵ��dir=0
    int bpts_num;            // �߽�����
};

// �����ɫ
struct DT_Color
{
    int r, g, b;
};

// ���鶨��
typedef std::vector<DT_Point> PointArray;
typedef std::vector<DT_Edge> EdgeArray;
typedef std::vector<DT_Triangle> TriangleArray;
typedef std::vector<DT_Contour> ContourArray;
typedef std::vector<DT_Color> ColorArray;

// ��ʹ��
typedef std::vector<int> IntArray;
typedef std::vector<double> DoubleArray;