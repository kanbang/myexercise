#pragma once

#include "../MineGE/DataLink.h"

class InstrumentDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;        // 仪表名称
	CString mn;          // 型号(Model Number)
	int amount;          // 仪表数量
	bool qi;             // 是否通过计量检验(Quantitative Inspection)
};
