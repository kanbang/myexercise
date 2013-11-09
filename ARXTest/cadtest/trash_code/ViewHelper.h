#pragma once

#include "dlimexp.h"

// ��ͼ��������
class MINEGE_DLLIMPEXP ViewHelper
{
public:
	static bool AddView(const CString& view);                                   // �½���ͼ
	static bool RemoveView(const CString& view);                                // �л���ͼ

	static bool AddElement(const CString& view, const CString& element);        // ���Ԫ�ص���ͼ��
	static bool RemoveElement(const CString& view, const CString& element);     // ����ͼ��ɾ��Ԫ��
	static int FindElement(const CString& view, const CString& element);        // ������ͼ��Ԫ�ص�����λ��
	static void GetAllElements(const CString& view, AcStringArray& elements);   // ��ȡ��ͼ������Ԫ��
	static int CountElements(const CString& view);                              // ͳ����ͼ��Ԫ�ظ���	

	static void SwitchView(const CString& view);                                // �л���ͼ
};
