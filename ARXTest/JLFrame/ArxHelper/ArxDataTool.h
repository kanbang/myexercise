#pragma once

#include "dlimexp.h"

/* 
 * ��չ���ݣ�XData��ʵ���Ͼ���һ������List��
 * ��������ڵ���Դ���κ����͵����ݣ������������ͣ���
 * �������������������ַ����ȡ�
 * 
 * ÿ����չ���ݿ��԰����ܶ��appName����ЩappName������ע�ᵽARX��RegApp Table��
 * 
 */
// ���ݸ���������
class ARXHELPER_DLLIMPEXP ArxDataTool
{
public:
	/* 1) ��չ���ݲ��� */

	/* ע��appName�����ݿ��RegApp Table�� */
	static void RegAppName(AcDbDatabase* db, const CString& appName);
	/* ��ȡͼԪ��չ����appName��ĵ�index������ */
	static bool GetDataFromXData(AcDbObject* pObj, const CString& appName, int index, CString& value);
	/* ����ͼԪ��չ����appName��ĵ�index������ */
	static bool SetDataToXData(AcDbObject* pObj, const CString& appName, int index, const CString& value);
	/* ��ͼԪ��չ����appName��������һ������ */
	static void AddDataToXData(AcDbObject* pObj, const CString& appName);
	/* ��ͼԪ��չ����appName����ɾ����index������ */
	static void RemoveDataFromXData(AcDbObject* pObj, const CString& appName, int index);
	/* ����ͼԪ��չ����appName������������ */
	static void AddDataToObjects(const AcDbObjectIdArray& objIds, const CString& appName);
	/* �Ӷ��ͼԪ��չ����appName����ɾ����index������ */
	static void RemoveDataFromObjects(const AcDbObjectIdArray& objIds, const CString& appName, int index);

	/* 2) �ʵ���� */

	/* �жϴʵ��Ƿ���� */
	static bool IsDictExist(const CString& dictName);	
	/* ��Ӵʵ� */
	static void RegDict(const CString& dictName);

	/* 3) ͼ�β��� */

	/* �ж�ͼԪ�Ƿ�������typeƥ�� */
	static bool IsEqualType(const CString& type, const AcDbObjectId& objId, bool isDerivedFromParent=true);
	/* ��������ָ�����͵�ͼԪ(��Ļ�Ͽɼ���) */
	static void GetEntsByType(const CString& type, AcDbObjectIdArray& objIds, bool isDerivedFromParent=true);
	/* 
	 * ��������ָ�����͵�ͼԪ(��Ļ�ϲ��ɼ��ģ��ѱ�ɾ����)
	 * ��CAD��ͼ��ɾ�������������ջ�ͼ�ζ���ռ�õ��ڴ�
	 * CADֻ�ǽ�ͼ�α��Ϊɾ�����ڹر�ͼ�ε�ʱ��
	 * ���Ϊɾ����ͼ�β��ᱣ�浽dwg�ļ���
	 */
	static void GetErasedEntsByType(const CString& type, AcDbObjectIdArray& objIds, bool isDerivedFromParent=true);
};
