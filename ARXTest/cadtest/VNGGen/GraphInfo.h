#pragma once

#include <vector>
using namespace std;

struct GraphInfo
{
	double width;    // 图的宽度
	double height;   // 图的高度
	double ratio;    // 图的宽/高比例
};

struct NodeInfo
{
	int id;          // 节点id
	double x, y;     // 节点坐标
	double cx, cy;   // 椭圆横轴和纵轴
};

struct EdgeInfo
{
	int id;                  // 分支id
	int sn, tn;              // 始末节点
	vector<double> cnx, cny; // 控制点坐标
	double tx, ty;           // 文本标签坐标
};

typedef vector<NodeInfo*> NodeInfoArray;
typedef vector<EdgeInfo*> EdgeInfoArray;
