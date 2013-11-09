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
	static double GridCacl(const AcGePoint3dArray& datas, const AcGePoint3d& minPt, const AcGePoint3d& maxPt, int nDiameter = 20, double dSemivariance = 4);
	
	/*
	 * ʹ��Kriging��ֵ������(����)����(x,y)��zֵ
	 */
	static double PointCacl(const AcGePoint3dArray& datas, const AcGePoint3d& pt, double dSemivariance = 4);
	
	/* 
	 * ���յ�ֵ����Ƶ�ֵ��
	 */
	static void DrawCountour(const AcGePoint3dArray& datas, double dz);

	/* 
	 * ���ո������Ƶ�ֵ��
	 */
	static void DrawCountour2(const AcGePoint3dArray& datas, int n);

	/*
	 * ���Ʊ߽�
	 */
	static void DrawBoundary(const AcGePoint3dArray& datas);
};
