#pragma once

// Object Record Pool ==> ORP
// �����¼��
class ORPHelper
{
public:
	// ����������
	static void CreateORP();
	static void DeleteORP();

	// ���ÿ���״̬
	static void SetORPState(bool flag);
	// ��ȡ����״̬
	static bool getORPState();

	// ���ø���
	static int GetCount();
	// ���
	static void ClearAll();
	
	// ���
	static bool AddObject(const AcDbObjectId& objId);
	// ��ȡ
	static bool GetObject(AcDbObjectId& objId);
};
