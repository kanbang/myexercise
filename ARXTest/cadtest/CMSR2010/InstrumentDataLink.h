#pragma once

#include "../MineGE/DataLink.h"

class InstrumentDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;        // �Ǳ�����
	CString mn;          // �ͺ�(Model Number)
	int amount;          // �Ǳ�����
	bool qi;             // �Ƿ�ͨ����������(Quantitative Inspection)
};
