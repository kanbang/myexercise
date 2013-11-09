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
	static double GridCacl(const AcGePoint3dArray& datas, const AcGePoint3d& minPt, const AcGePoint3d& maxPt, int nDiameter = 20);
	
	/*
	 * 插值计算(估计)坐标(x,y)的z值
	 */
	static double PointCacl(const AcGePoint3dArray& datas, const AcGePoint3d& pt);
	
	/* 
	 * 获取点数组中的最大z值和最小z值
	 */
	static void GetMinMaxZValue(const AcGePoint3dArray& datas, double& minZ, double& maxZ);

	static void PreprocessPointSet(AcGePoint3dArray& bounds, const AcGePoint3dArray& datas, AcGePoint3dArray& new_datas);

	/* 
	 * 按照个数绘制等值线
	 */
	static void DrawCountourByNum(AcGePoint3dArray& bounds, const AcGePoint3dArray& pts, double minZ, double maxZ, int n);

	/* 
	 * 按照指定的最大、最小、等差绘制等值线
	 */
	static void DrawCountourByDeltaZ(AcGePoint3dArray& bounds, const AcGePoint3dArray& pts, double minZ, double maxZ, double dz);

	/*
	 * 绘制等值线填充
	 * 如果colors.length>0, 那么colors.length() - zValues.length() 必须大于等于 1
	 */
	static void DrawCountourFill(AcGePoint3dArray& bounds, const AcGePoint3dArray& pts, const AcGeDoubleArray& zValues, const AcArray<COLORREF>& colors);

	/*
	 * 绘制比色板
	 */
	static void DrawColorPlate();

	/* 
	 * 获取图层上的等值线图元
	 */
	static void GetContourGEOnLayer(const CString& layer, const CString& type, AcDbObjectIdArray& objIds);
};
