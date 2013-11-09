#pragma once

// 包含"点"定义
#include "Point.h"
using namespace JL;

// 边
struct Edge
{
    int s, t;
};

// 三角形
struct Triangle
{
    int p1, p2, p3; // 3个点
};

typedef std::vector<Edge> EdgeArray;
typedef std::vector<Triangle> TriangleArray;