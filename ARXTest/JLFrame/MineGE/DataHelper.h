#pragma once

#include "dlimexp.h"

// DataHelper�ṩ��2����̬�������ڶ�д��������
// ���ǵ�ʵ��Ч�ʲ����ߣ������ڶ�ȡ�����ֶε����
// �����Ҫ������ȡһ��ͼԪ�Ĵ����ֶΣ����Կ���ʹ��DataLink��
class MINEGE_DLLIMPEXP DataHelper
{
public:
	// ��ȡ���ݶ���(DataObject)
	// ���objId��ʾһ��MineGEͼԪ���򷵻�ͼԪ���������ݶ���(DataObject)
	static bool GetDataObject(const AcDbObjectId& objId, AcDbObjectId& dObjId);

	// �ж��Ƿ����ݶ���
	static bool IsDataObject(const AcDbObjectId& objId);

	// ��ȡ��������
	// ע��objId������һ��MineGEͼԪ�����ݶ���(DataObject)
	static bool GetTypeName(const AcDbObjectId& objId, CString& type);

	// ��ȡ��������
	// ����ֶβ����ڣ�����false
	// ע��objId������һ��MineGEͼԪ�����ݶ���(DataObject)
	static bool GetPropertyData(const AcDbObjectId& objId, const CString& fieldName, CString& value);

	// �޸���������
	// ����ֶβ����ڣ�����false
	// ע��objId������һ��MineGEͼԪ�����ݶ���(DataObject)
	static bool SetPropertyData(const AcDbObjectId& objId, const CString& fieldName, const CString& value);

	// ������������
	// ע��objId������һ��MineGEͼԪ�����ݶ���(DataObject)
	static bool CopyPropertyData(const AcDbObjectId& sourceObjId, const AcDbObjectId& targetObjId);
};
