#pragma once

#include "dlimexp.h"

/* 
 * ARX���õ����Լ��Զ����඼��Ҫ��ӵ�ARX�����νṹ��(Class Hierarchy)
 * ARX���ఴ��������ϵ���ɲ�νṹ��(Class Hierarchy) 
 * ���arx-->ѡ��O-->ѡ��CL��CAD�������������Class Hierarchy(��F2�鿴�Ŵ������д���)
 *
 * ARXʵ����һ��RTTI(run time type infomation, ����ʱ������Ϣ)��
 * ÿ��ARX�඼��Ӧһ��ArxClass����
 * RRTI��һЩ�Ƚϼ򵥵���;��
 *	  1) ����һ�����󣬻�ȡ������������ַ�������
 *    2) �ж�2������֮���Ƿ����������ϵ
 *    3) ��ȡ��������и�������(��ARX�л����ϲ�ʹ�ö��ؼ̳�)
 *    4) ��ȡ�����������������
 *	  5) �����ַ����������ƣ���̬����һ������ָ��(���Ӧ��������)
 *
 * ARX���νṹ����Ӧ��ARX���ݽṹ��һ���ʵ�(AcRxDictionary)
 * ��ͨ��acrxClassDictionary������ȡ����ָ��
 * ����͵��÷���ͨ��acrxClassDictionary�ṩ�ķ�����ѯ���Ͷ�Ӧ��ArxClass����
 *    AcRxClass* pClass = AcRxClass::cast(acrxClassDictionary->at(_T("Tunnel")));
 * ע�⣺����ֻ��Ҫ����һ���ַ���
 */
class ARXHELPER_DLLIMPEXP ArxClassHelper
{
public:
	/* �ж������Ƿ���Ч������arx����ϵ�ṹ���Ƿ���ע�� */
	/* ��arx����(AcDbObject/AcDbEntity)�������඼��ע�ᵽarx����ϵ�ṹ�� */
	static bool IsValidClass(const CString& type);
	
	/* �ж������Ƿ������(�����಻��ʵ����) */
	/* ������ͨ���ǹ��캯��Ϊprotected��������������麯�� */
	static bool IsAbstractClass(const CString& type);
	
	/* �ж�����֮���Ƿ����������ϵ */
	static bool IsDerivedFrom(const CString& type, const CString& parentType);
	
	/* ��ȡ���͵�dxf����(��arx�����������඼��һ��dxf���ƣ���������������) */
	/* ��cad�а����ͣ����һ��ͼ���������ʾͼ�ε�dxf���ƣ�����AcDbLine��dxf���ƾ��ǡ�ֱ�ߡ� */
	static bool GetDxfName(const CString& type, CString& dxfName);
	
	/* �������͵����л��� */
	static void GetClassHierarchy(const CString& type, AcStringArray& types, bool findAbstractType);
	
	/* �������͵��������� */
	static bool GetChildClass(const CString& type, AcStringArray& types);	
	
	/* �������͵ľ�������ķǳ������ */
	static bool GetTopParentClass(const CString& type, CString& parentType);
	
	/* �������͵����зǳ������(��������Ļ���...) */
	static bool GetAllTopParentClass(const CString& root, AcStringArray& types);

	/* ���һ���root���������� */
	static void GetArxClassTypes(const CString& root, AcStringArray& types, bool findAbstractType);		

	/* �����������ƣ���̬�������󲢷���ָ�� */
	static AcDbObject* CreateObjectByType(const CString& type);
};
