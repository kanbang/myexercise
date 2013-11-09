#pragma once

enum EdgeType
{
	ET_COMMON   = 0,  // ��ͨ���
	ET_USEAIR   = 1,  // �����÷�ص�(�زɹ����桢����������������)
	ET_VIRTUAL  = 2   // �����֧
};

struct EdgeData
{
	EdgeData() : et(ET_COMMON), vt(0) {}

	AcDbObjectId objId;
	EdgeType et;   // ��֧����(Ĭ��Ϊ��ͨ���-ET_COMMON)
	int vt;           // ͨ������(Ĭ��Ϊ����-0)
};
