#pragma once

#include "dlimexp.h"

// DataHelper�ṩ��2����̬�������ڶ�д��������
// ���ǵ�ʵ��Ч�ʲ����ߣ������ڶ�ȡ�����ֶε����
// �����ҪҪ������ȡһ��ͼԪ�Ĵ����ֶΣ����Կ���ʹ��DataLink��
class MINEGE_DLLIMPEXP DataHelper
{
public:
	// ��ȡ���ݶ���,�����������ͼԪ(MineGE)������רҵģ��ͼԪ(ModelGE)
	static bool GetDataObject(const AcDbObjectId& objId, AcDbObjectId& dObjId);

	// �ж��Ƿ����ݶ���
	static bool IsDataObject(const AcDbObjectId& objId);

	// ��ȡ���ݶ������������(���ֶ��й�)
	static bool GetTypeName(const AcDbObjectId& objId, CString& type);

	// ��ȡ��������
	// ����ֶβ����ڣ�����false
	static bool GetPropertyData(const AcDbObjectId& objId, const CString& fieldName, CString& value);

	// �޸���������
	// ����ֶβ����ڣ�����false
	static bool SetPropertyData(const AcDbObjectId& objId, const CString& fieldName, const CString& value);

	// ������������
	static void CopyPropertyData(const AcDbObjectId& sourceObjId, const AcDbObjectId& targetObjId);
};
