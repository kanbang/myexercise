#pragma once

#include "../MineGE/DataLink.h"

class GasDataLink : public DataLink
{
protected:
	virtual void regDatas()
	{
		linkDoubleData(name, &z);
	}

public:
	double z;     // ��˹��zֵ(��˹��������˹ѹ������˹ӿ����)
	CString name; // Ҫ��ȡ���ֶ�����(�ⲿ����!!!)
};
