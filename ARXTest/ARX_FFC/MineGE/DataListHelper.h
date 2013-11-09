#pragma once

#include "dlimexp.h"

class MINEGE_DLLIMPEXP StringListHelper
{
public:
	static void GetAllNames(AcStringArray& names);
	static bool AddString(const CString& name, const CString& strValue);	// ʵ��Ч�ʽϵ�
	static bool AddStringList(const CString& name, const AcStringArray& strList);
	static bool GetStringList(const CString& name, AcStringArray& strList);
	static void RemoveStringList(const CString& name);
};

class MINEGE_DLLIMPEXP IntStrListHelper
{
public:
	static void GetAllNames(AcStringArray& names);
	static bool AddIntStrPair(const CString& name, int intValue, const CString& strValue);	// ʵ��Ч�ʽϵ�
	static bool AddIntStrList(const CString& name, const AcDbIntArray& intList, const AcStringArray& strList);
	static bool GetIntStrList(const CString& name, AcDbIntArray& intList, AcStringArray& strList);
	static void RemoveIntStrList(const CString& name);
};

// ���ݶ����б�����
// ��"����ˮƽ"��"ú��"֮�����Ϣ��ÿһ����Ϣ���������������
class MINEGE_DLLIMPEXP DataObjectListHelper
{
public:
	static void GetAllNames(AcStringArray& names);
	static bool AddObject(const CString& name, const CString& value);
	static bool RemoveObject(const CString& name, const CString& value);
	static bool RemoveAllObject(const CString& name);
	static bool GetObjectNameList(const CString& name, AcStringArray& values);
	static bool IsObjectListType(const CString& name);
	static bool GetObjectId(const CString& name, const CString& value, AcDbObjectId& objId);
};

// ��DataObjectListHelper����ͬ��ÿ��name��ֻ����1�����ݶ���
// Ĭ�ϵ�value <==> SINGLE_DATA_OBJECT_DEF_NAME���μ�config.h
// ��Ҫ��������ȫ��Ψһ����Ϣ������"����Ϣ", "������Ŀ��Ϣ"��
// ��Щ��Ϣ���ڵ�ǰ��ͼ����˵��Ψһ�ģ�����Ҫ�ж��
class MINEGE_DLLIMPEXP SingleDataObjectHelper
{
public:
	static void GetAllNames(AcStringArray& names);
	static bool AddObject(const CString& name);
	static bool RemoveObject(const CString& name);
	static bool GetObjectId(const CString& name, AcDbObjectId& objId);
};