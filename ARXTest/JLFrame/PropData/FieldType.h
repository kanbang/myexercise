#pragma once

// ��������ö��
enum DATA_TYPE
{
	DT_STRING   = 0, // �ַ���
	DT_INT      = 1, // ����
	DT_NUMERIC  = 2, // ������
	DT_BOOL     = 3, // ��������
	DT_DATE     = 4, // ��������
	DT_LIST     = 5  // �б�����(�������ɢ����)
};

// �б���������
enum LIST_TYPE
{
	LT_STRING = 0, // �ַ����б�
	LT_INT    = 1, // �����б�
	LT_OBJECT = 2  // AcDbObject�б�
};