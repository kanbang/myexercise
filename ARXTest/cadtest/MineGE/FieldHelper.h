#pragma once

#include "dlimexp.h"

// �����ֶι�������
class MINEGE_DLLIMPEXP FieldHelper
{
public:
	static bool AddField(const CString& type, const CString& field);        // ����ֶ�
	static bool RemoveField(const CString& type, const CString& field);     // ɾ���ֶ�
	static void RemoveAllFields(const CString& type);                       // ɾ�������ֶ�
	static int FindField(const CString& type, const CString& field);        // �����ֶε�����λ��
	static void GetAllFields(const CString& type, AcStringArray& fields);   // ��ȡͼԪ�������ֶ�
	static int CountFields(const CString& type);                            // ͳ��ͼԪ���ֶθ���
	static void GetAllRegTypes(AcStringArray& types);		                // ��ȡ��ǰ��ע���ͼԪ����
};
