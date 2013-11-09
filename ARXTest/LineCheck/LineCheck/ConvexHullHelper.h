#pragma once

/* 凸包算法 */
/* 3d的实现调用的是2d的方法，两者没有什么区别 */

/* 使用Graham-Scan算法 */
extern bool Graham_ConvexHull_2D(const AcGePoint2dArray& V, AcGePoint2dArray& H);
extern bool Graham_ConvexHull_3D(const AcGePoint3dArray& V, AcGePoint3dArray& H);

/* 使用Melkman算法 */
extern bool Melkman_ConvexHull_2D(const AcGePoint2dArray& V, AcGePoint2dArray& H);
extern bool Melkman_ConvexHull_3D(const AcGePoint3dArray& V, AcGePoint3dArray& H);

extern bool ClosestPoint_2D(const AcGePoint2dArray& V, AcGePoint2d& pt1, AcGePoint2d& pt2);
extern bool ClosestPoint_3D(const AcGePoint3dArray& V, AcGePoint3d& pt1, AcGePoint3d& pt2);
