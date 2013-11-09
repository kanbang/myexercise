#pragma once

#include "dlimexp.h"

// ARX������
class ARXHELPER_DLLIMPEXP ArxUtilHelper
{
public:
	// ���½���ͼԪ��ӵ�CADͼ�����ݿ�
	static bool PostToModelSpace(AcDbEntity* pEnt);

	// �����ѡ��һ��ʵ��
	// ������msg��ʾ��cad��������ʾ����ʾ��Ϣ
	static AcDbObjectId SelectObject(const CString& msg);

	// ��ȡ��ǰ��ѡ���ͼԪ(PickSet)
	static void GetPickSetEntity(AcDbObjectIdArray& objIds);

	// ����ѡ��һ��������
	// ������
	//		msg -- ��ʾ��cad��������ʾ����ʾ��Ϣ
	//      pt  --  �����û�ѡ��ĵ�����
	static bool PromptPt(const CString& msg, AcGePoint3d& pt);

	// ����ѡ��һ��������
	// ������
	//		msg    -- ��cad��������ʾ����ʾ��Ϣ
	//		basePt -- ���㣬�û��ƶ����ʱ��
	//                �Զ�����һ����basePt����굱ǰ�����Ƥ��
	//      pt     -- �����û�ѡ��ĵ�����
	static bool PromptPt2(const CString& msg, const AcGePoint3d& basePt, AcGePoint3d& pt);

	// ��������v��x����ʱ����ת�Ƕ�
	static double AngleToXAxis(const AcGeVector3d& v);

	// ��ͣ
	// CAD�����л������ʾ��Ϣ��"�밴�س���ո������..."
	static void Pause();

	// �Ƿ�ָ�����͵�ͼԪ
	// ע����ArxDataTool::IsEqualType()�����ڹ�������һ����
	//      �μ�ArxDataTool.h
	static bool IsEqualType(const CString& type, const AcDbObjectId& objId, bool isDerivedFromParent=true);

	/* ����4��������ʱû��ʵ�ʵ���; */
	// ��ʱ��һ����ɫ��ʾһ��ͼԪ
	// �Ự�����󣬻ָ�ԭ����ɫ
	static void ShowEntityWithColor(const AcDbObjectId& objId, unsigned short colorIndex);
	// ��ʱ��һ����ɫ��ʾ���ͼԪ
	// �Ự�����󣬻ָ�ԭ����ɫ
	static void ShowEntitiesWithColor(AcDbObjectIdArray& objIds, unsigned short colorIndex);
	// ��ʱ�ö�����ɫ��ʾ���ͼԪ
	// �Ự�����󣬻ָ�ԭ����ɫ
	static void ShowEntitiesWithColor2(AcDbObjectIdArray& objIds, const AcArray<Adesk::UInt16>& colors);
	// �����н����������ͼԪ
	static void BrowserEntities(const AcDbObjectIdArray& objIds);

	// �����Ƕ�
	// ���ؽ������ֵ��[0, 2*PI]
	// �������������С�ĽǶ�(��������������ֵ���Գ���[0, 2*PI]���䷶Χ)
	static double AdjustAngle(double angle);

	/* string<-->int, double֮���ת�� */

	// �ַ���ת�ɸ�����
	static bool StringToDouble(const CString& value, double& v);
	// �ַ���ת������
	static bool StringToInt(const CString& value, int& v);
	// ������ת�����ַ���
	static void DoubleToString(double v, CString& value);
	// ����ת�����ַ���
	static void IntToString(int v, CString& value);
	
	// �ж��ַ����Ƿ���һ���Ϸ���������
	// ��ʽ[+/-][0-9]
	static bool IsInteger(LPCTSTR pSrc);
	
	// �ж��ַ����Ƿ���һ���Ϸ��ĸ�������
	// ��ʽ[+/-][0-9][.][0-9]	
	static bool IsNumeric(LPCTSTR pSrc);

	// ����GUID�ַ���
	// GUID�ڿռ��Ϻ�ʱ���Ͼ���Ψһ�ԣ���֤ͬһʱ�䲻ͬ�ط����������ֲ�ͬ�� 
	// �����ϵ��κ���̨����������������ظ��� GUID ֵ
	// ���磺{9E3549BB-9144-42ea-8045-83BC250A2E8C}
	static bool NewGUID(CString& strGUID);
};
