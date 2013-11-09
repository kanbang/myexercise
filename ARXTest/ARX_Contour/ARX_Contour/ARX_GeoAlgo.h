#pragma once

/* ͹���㷨 */
/* ʹ��Graham-Scan�㷨 */
extern bool Graham_ConvexHull_3D(const AcGePoint3dArray& V, AcGePoint3dArray& H);

/* ͹���㷨 */
/* ʹ��Melkman�㷨 */
extern bool Melkman_ConvexHull_3D(const AcGePoint3dArray& V, AcGePoint3dArray& H);

/* ���ҵ㼯�о��������2�� */
extern bool ClosestPoint_3D(const AcGePoint3dArray& V, AcGePoint3d& pt1, AcGePoint3d& pt2);

/* �жϵ��Ƿ��ڶ������ */
extern bool IsPointInPolygon(const AcGePoint3d& p, const AcGePoint3dArray& ptPolygon);

/* ����������� */
extern double PolygonArea(const AcGePoint3dArray& polygon);

/* ������������ */
extern AcGePoint3d PolygonCentroid(const AcGePoint3dArray& polygon);
