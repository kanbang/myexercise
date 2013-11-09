#pragma once

#include "GeoDef.h"

// �ο���
// 1) http://hi.baidu.com/longchengjiang/blog/item/2cdde73fb897cac07d1e713f.html
// 2) http://www.cs.cmu.edu/~quake/triangle.html
// 3) http://en.wikipedia.org/wiki/Delaunay_triangulation
// 4) http://en.wikipedia.org/wiki/Contour_line

/*
* ��ֵ�����ݸ�ʽ��
* ���磺׷��3��zֵ������õ�4����ֵ��
*		  ���е�2��zֵ��2����ֵ��
* ���ݣ�
*      zֵ  ��ֵ������	��ֵ�����
*		1.3		1			5
*		3.2		2			6, 4
*		5.1		1			12
* ��:
*		zValues = {1.3, 3.2, 5.1}
*		cnpts   = { ... } ������5+6+4+12=25
*		cna   = {(1,5), (2,6), (2,4), (1,12)}
*/

// ͨ��triangle.exe+�ļ�io����������������
extern "C" __declspec(dllexport)
void CDT_EXE(
    const PointArray& pts,
    const EdgeArray& segments,
    const PointArray& holes,
    EdgeArray& ea,
    TriangleArray& ta);

// ֱ�ӵ���triangle�ĺ����ӿ�
extern "C" __declspec(dllexport)
void CDT_LIB(
    const PointArray& pts,
    const EdgeArray& segments,
    const PointArray& holes,
    EdgeArray& ea,
    TriangleArray& ta);

extern "C" __declspec(dllexport)
void TraceContours(
    /* �� */
    const PointArray& pts,
    /* �� */
    const EdgeArray& ea,
    /* ������ */
    const TriangleArray& ta,
    /* Ҫ׷�ٵ�zֵ */
    const DoubleArray& zValues,
    /* ���е�ֵ�ߵ㶼����һ������ */
    PointArray& cnpts,
    /* ��¼��ֵ����Ϣ(�����Լ�����λ��) */
    ContourArray& cna);

extern "C" __declspec(dllexport)
void Clip(
    /* ׷�ٵõ��ĵ�ֵ�ߵ� */
    const PointArray& cnpts,
    /* ��¼��ֵ����Ϣ(�����Լ�����λ��) */
    const ContourArray& cna,
    /* �߽����� */
    const PointArray& bpts,
    /* ��¼�ü���ĵ�ֵ�ߺ���Ϣ */
    PointArray& all_cnpts,
    ContourArray& all_cna);

extern "C" __declspec(dllexport)
void SearchBoundary(
    /* �� */
    const PointArray& pts,
    /* �� */
    const EdgeArray& ea,
    /* ������ */
    const TriangleArray& ta,
    /* �����Χ�߽� */
    PointArray& bpts);

// �⻬�㷨(���û���������û��ʹ��������������������������������)
// ע��3������Ч������̫��,����С�ǶȵĴ����ã����ײ�������
extern "C" __declspec(dllexport)
void SmoothPoints(
    const PointArray& inPts,
    PointArray& outPts,
    unsigned int N=1);

// �պϵ�ֵ�߲�������ֵ������
extern "C" __declspec(dllexport)
void EncloseContour_BuildTopolgy(
    /* ׷�ٵõ��ĵ�ֵ�ߵ� */
    const PointArray& cnpts,
    /* ��¼��ֵ����Ϣ(�����Լ�����λ��) */
    const ContourArray& cna,
    /* �߽����� */
    const PointArray& bpts,
    /* ��¼�ϲ���ĵ�ֵ�ߺ���Ϣ */
    PointArray& all_cnpts,
    ContourArray& all_cna,
    IntArray& sortPos);

extern "C" __declspec(dllexport)
void SearchLabelPostion(const PointArray& cnpts, DT_Point& tpt, double& angle);

extern "C" __declspec(dllexport)
void SearchLabelPostions(const PointArray& cnpts, PointArray& tpts);
