#pragma once

// ����"��"����
#include "Point.h"
using namespace JL;

// ��
struct Edge
{
    int s, t;
};

// ������
struct Triangle
{
    int p1, p2, p3; // 3����
};

typedef std::vector<Edge> EdgeArray;
typedef std::vector<Triangle> TriangleArray;