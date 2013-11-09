#pragma once

#include "GeoDef.h"

// 参考：
// 1) http://hi.baidu.com/longchengjiang/blog/item/2cdde73fb897cac07d1e713f.html
// 2) http://www.cs.cmu.edu/~quake/triangle.html
// 3) http://en.wikipedia.org/wiki/Delaunay_triangulation
// 4) http://en.wikipedia.org/wiki/Contour_line

/*
* 等值线数据格式：
* 例如：追踪3个z值，计算得到4条等值线
*		  其中第2个z值有2条等值线
* 数据；
*      z值  等值线条数	等值点个数
*		1.3		1			5
*		3.2		2			6, 4
*		5.1		1			12
* 则:
*		zValues = {1.3, 3.2, 5.1}
*		cnpts   = { ... } 个数：5+6+4+12=25
*		cna   = {(1,5), (2,6), (2,4), (1,12)}
*/

// 通过triangle.exe+文件io操作生成三角网格
extern "C" __declspec(dllexport)
void CDT_EXE(
    const PointArray& pts,
    const EdgeArray& segments,
    const PointArray& holes,
    EdgeArray& ea,
    TriangleArray& ta);

// 直接调用triangle的函数接口
extern "C" __declspec(dllexport)
void CDT_LIB(
    const PointArray& pts,
    const EdgeArray& segments,
    const PointArray& holes,
    EdgeArray& ea,
    TriangleArray& ta);

extern "C" __declspec(dllexport)
void TraceContours(
    /* 点 */
    const PointArray& pts,
    /* 边 */
    const EdgeArray& ea,
    /* 三角形 */
    const TriangleArray& ta,
    /* 要追踪的z值 */
    const DoubleArray& zValues,
    /* 所有等值线点都放在一个数组 */
    PointArray& cnpts,
    /* 记录等值线信息(个数以及索引位置) */
    ContourArray& cna);

extern "C" __declspec(dllexport)
void Clip(
    /* 追踪得到的等值线点 */
    const PointArray& cnpts,
    /* 记录等值线信息(个数以及索引位置) */
    const ContourArray& cna,
    /* 边界多边形 */
    const PointArray& bpts,
    /* 记录裁剪后的等值线和信息 */
    PointArray& all_cnpts,
    ContourArray& all_cna);

extern "C" __declspec(dllexport)
void SearchBoundary(
    /* 点 */
    const PointArray& pts,
    /* 边 */
    const EdgeArray& ea,
    /* 三角形 */
    const TriangleArray& ta,
    /* 最大外围边界 */
    PointArray& bpts);

// 光滑算法(采用基数样条，没有使用三次样条、贝塞尔样条或其它样条)
// 注：3次样条效果不是太好,对于小角度的处理不好，容易产生交叉
extern "C" __declspec(dllexport)
void SmoothPoints(
    const PointArray& inPts,
    PointArray& outPts,
    unsigned int N=1);

// 闭合等值线并构建等值线拓扑
extern "C" __declspec(dllexport)
void EncloseContour_BuildTopolgy(
    /* 追踪得到的等值线点 */
    const PointArray& cnpts,
    /* 记录等值线信息(个数以及索引位置) */
    const ContourArray& cna,
    /* 边界多边形 */
    const PointArray& bpts,
    /* 记录合并后的等值线和信息 */
    PointArray& all_cnpts,
    ContourArray& all_cna,
    IntArray& sortPos);

extern "C" __declspec(dllexport)
void SearchLabelPostion(const PointArray& cnpts, DT_Point& tpt, double& angle);

extern "C" __declspec(dllexport)
void SearchLabelPostions(const PointArray& cnpts, PointArray& tpts);
