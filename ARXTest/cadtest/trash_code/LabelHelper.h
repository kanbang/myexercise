#pragma once

#include "dlimexp.h"

// ��ǩ����������
// ���Ը��κ�ͼԪʵ��(��AcDbObject����)��ӱ�ǩ
class MINEGE_DLLIMPEXP LabelHelper
{
public:
	// ���һ����ǩ
	static void AddLabel(const AcDbObjectId& objId, const CString& label);

	// ��Ӷ����ǩ
	static void AddMoreLabels(const AcDbObjectId& objId, const AcStringArray& labels);

	// ɾ��һ����ǩ
	static void RemoveLabel(const AcDbObjectId& objId, const CString& label);
	
	// ɾ�������ǩ
	static void RemoveMoreLabels(const AcDbObjectId& objId, const AcStringArray& labels);

	// ɾ�����б�ǩ
	static void RemoveAllLabels(const AcDbObjectId& objId);

	// ��ȡ���еı�ǩ
	static void GetAllLabels(const AcDbObjectId& objId, AcStringArray& labels);

	// �Ƿ���б�ǩ
	static bool HasLabel(const AcDbObjectId& objId, const CString& label);
};
