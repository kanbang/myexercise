#pragma once

#include "../MineGE/DataLink.h"

// ���������ľ�Ͳ(���)��������
class LiftShaftDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;         // ����
	double v;             // ����
	int ft;              // ��������
	int sp;              // ��Ͳ��������

	int vt;              // ͨ������(���ֶ�,�ɳ���ֵ)
	bool hasLift;        // ��������ú̿���豸
};
