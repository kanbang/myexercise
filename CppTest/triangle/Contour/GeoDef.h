#pragma once

#include <vector>

// 点
struct DT_Point
{
    double x, y, z;
};

// 边
struct DT_Edge
{
    int s, t;
};

// 三角形
struct DT_Triangle
{
    int p1, p2, p3; // 3个点
};

// 等值线
struct DT_Contour
{
    int zi;                  // 对应的z值索引(如果是边界, 则等于-1)
    int cnpts_num;           // 等值线点个数(如果是边界,则等于0)

    int dir;                 // 闭合等值线方向(由外->内变小dir=1, 则dir=-1), 开放等值线dir=0
    int bpts_num;            // 边界点个数
};

// 填充颜色
struct DT_Color
{
    int r, g, b;
};

// 数组定义
typedef std::vector<DT_Point> PointArray;
typedef std::vector<DT_Edge> EdgeArray;
typedef std::vector<DT_Triangle> TriangleArray;
typedef std::vector<DT_Contour> ContourArray;
typedef std::vector<DT_Color> ColorArray;

// 简化使用
typedef std::vector<int> IntArray;
typedef std::vector<double> DoubleArray;