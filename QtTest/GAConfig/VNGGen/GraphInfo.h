#pragma once

#include <vector>
using namespace std;

struct GraphInfo
{
	double width;    // 图的宽度
	double height;   // 图的高度
	double ratio;    // 图的宽/高比例
};

/* 
 * 节点id是虚拟的编号
 * 与实际节点编号不相同
 * 通过lemon graph的id()方法得到
 */
struct NodeInfo
{
	int id;          // 节点id
	double x, y;     // 节点坐标
	double cx, cy;   // 椭圆横轴和纵轴
};

/* 
 * 分支id、始末节点sn, tn均是虚拟的编号
 * 与实际节点编号不相同,
 * 通过lemon graph的id()方法得到
 */
struct EdgeInfo
{
	int id;                  // 分支id
	int sn, tn;              // 始末节点
	vector<double> cnx, cny; // 控制点坐标
	double tx, ty;           // 文本标签坐标
};

typedef vector<NodeInfo*> NodeInfoArray;
typedef vector<EdgeInfo*> EdgeInfoArray;
