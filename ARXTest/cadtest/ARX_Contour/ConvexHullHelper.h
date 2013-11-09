#pragma once

/* ͹���㷨 */
/* 3d��ʵ�ֵ��õ���2d�ķ���������û��ʲô���� */

/* ʹ��Graham-Scan�㷨 */
extern bool Graham_ConvexHull_2D(const AcGePoint2dArray& V, AcGePoint2dArray& H);
extern bool Graham_ConvexHull_3D(const AcGePoint3dArray& V, AcGePoint3dArray& H);

/* ʹ��Melkman�㷨 */
extern bool Melkman_ConvexHull_2D(const AcGePoint2dArray& V, AcGePoint2dArray& H);
extern bool Melkman_ConvexHull_3D(const AcGePoint3dArray& V, AcGePoint3dArray& H);

extern bool ClosestPoint_2D(const AcGePoint2dArray& V, AcGePoint2d& pt1, AcGePoint2d& pt2);
extern bool ClosestPoint_3D(const AcGePoint3dArray& V, AcGePoint3d& pt1, AcGePoint3d& pt2);
