#pragma once

/* 凸包算法 */
/* 使用Graham-Scan算法 */
extern bool Graham_ConvexHull_3D(const AcGePoint3dArray& V, AcGePoint3dArray& H);

/* 凸包算法 */
/* 使用Melkman算法 */
extern bool Melkman_ConvexHull_3D(const AcGePoint3dArray& V, AcGePoint3dArray& H);

/* 查找点集中距离最近的2点 */
extern bool ClosestPoint_3D(const AcGePoint3dArray& V, AcGePoint3d& pt1, AcGePoint3d& pt2);

/* 判断点是否在多边形内 */
extern bool IsPointInPolygon(const AcGePoint3d& p, const AcGePoint3dArray& ptPolygon);

/* 计算多边形面积 */
extern double PolygonArea(const AcGePoint3dArray& polygon);

/* 计算多边形形心 */
extern AcGePoint3d PolygonCentroid(const AcGePoint3dArray& polygon);
