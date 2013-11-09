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
	int zi;                  // 对应的z值索引
	int cnpts_num;           // 等值线点个数
	int dir;                 // 表示闭合等值线方向(z值由外->内变小dir=1, 则dir=-1), 开放等值线dir=0
};

// 当等值线与边界闭合的时候, 扩展等值线信息
struct DT_ContourExt
{
	int bpts_num;            // 边界点个数
	int next_contour;        // 指向面积更大的等值线
};

// 等值线标注
struct DT_Label
{
	DT_Point pt;
	double angle;
};

// 数组定义
typedef std::vector<DT_Point> PointArray;
typedef std::vector<DT_Edge> EdgeArray;
typedef std::vector<DT_Triangle> TriangleArray;
typedef std::vector<DT_Contour> ContourArray;
typedef std::vector<DT_ContourExt> ContourExtArray;
typedef std::vector<DT_Label> LabelArray;

// 简化使用
typedef std::vector<int> IntArray;
typedef std::vector<double> DoubleArray;