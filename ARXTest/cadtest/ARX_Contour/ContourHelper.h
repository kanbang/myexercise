#pragma once

class ContourHelper
{
public:
	/*
	 * 输入：
	 *		离散数据     -- datas
	 *		矩形框的边界 -- minPt ~ maxPt
	 *		划分的网格行(列)个数 -- nDiameter
	 *		使用Kriging插值法，指定半平方差因子 -- dSemivariance
	 * 输出：
	 *		矩形区域的z值平均值
	 *
	 * 计算过程：划分网格，计算网格行列交点的z值，
	 *			   求所有的z值之和，并除以网格的总个数(nDiameter^2)
	 */
	static double GridCacl(const AcGePoint3dArray& datas, const AcGePoint3d& minPt, const AcGePoint3d& maxPt, int nDiameter = 20, double dSemivariance = 4);
	
	/*
	 * 使用Kriging插值法计算(估计)坐标(x,y)的z值
	 */
	static double PointCacl(const AcGePoint3dArray& datas, const AcGePoint3d& pt, double dSemivariance = 4);
	
	/* 
	 * 按照等值差绘制等值线
	 */
	static void DrawCountour(const AcGePoint3dArray& datas, double dz);

	/* 
	 * 按照个数绘制等值线
	 */
	static void DrawCountour2(const AcGePoint3dArray& datas, int n);

	/*
	 * 绘制边界
	 */
	static void DrawBoundary(const AcGePoint3dArray& datas);
};
