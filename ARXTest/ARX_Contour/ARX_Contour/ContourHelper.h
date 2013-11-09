#pragma once

class ContourHelper
{
public:
	/*
	 * ���룺
	 *		��ɢ����     -- datas
	 *		���ο�ı߽� -- minPt ~ maxPt
	 *		���ֵ�������(��)���� -- nDiameter
	 *		ʹ��Kriging��ֵ����ָ����ƽ�������� -- dSemivariance
	 * �����
	 *		���������zֵƽ��ֵ
	 *
	 * ������̣��������񣬼����������н����zֵ��
	 *			   �����е�zֵ֮�ͣ�������������ܸ���(nDiameter^2)
	 */
	static double GridCacl(const AcGePoint3dArray& datas, const AcGePoint3d& minPt, const AcGePoint3d& maxPt, int nDiameter = 20);
	
	/*
	 * ��ֵ����(����)����(x,y)��zֵ
	 */
	static double PointCacl(const AcGePoint3dArray& datas, const AcGePoint3d& pt);
	
	/* 
	 * ��ȡ�������е����zֵ����Сzֵ
	 */
	static void GetMinMaxZValue(const AcGePoint3dArray& datas, double& minZ, double& maxZ);

	static void PreprocessPointSet(AcGePoint3dArray& bounds, const AcGePoint3dArray& datas, AcGePoint3dArray& new_datas);

	/* 
	 * ���ո������Ƶ�ֵ��
	 */
	static void DrawCountourByNum(AcGePoint3dArray& bounds, const AcGePoint3dArray& pts, double minZ, double maxZ, int n);

	/* 
	 * ����ָ���������С���Ȳ���Ƶ�ֵ��
	 */
	static void DrawCountourByDeltaZ(AcGePoint3dArray& bounds, const AcGePoint3dArray& pts, double minZ, double maxZ, double dz);

	/*
	 * ���Ƶ�ֵ�����
	 * ���colors.length>0, ��ôcolors.length() - zValues.length() ������ڵ��� 1
	 */
	static void DrawCountourFill(AcGePoint3dArray& bounds, const AcGePoint3dArray& pts, const AcGeDoubleArray& zValues, const AcArray<COLORREF>& colors);

	/*
	 * ���Ʊ�ɫ��
	 */
	static void DrawColorPlate();

	/* 
	 * ��ȡͼ���ϵĵ�ֵ��ͼԪ
	 */
	static void GetContourGEOnLayer(const CString& layer, const CString& type, AcDbObjectIdArray& objIds);
};
