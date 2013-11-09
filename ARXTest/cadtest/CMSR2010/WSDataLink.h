#pragma once

#include "../MineGE/DataLink.h"

class WSDataLink : public DataLink
{
public:
	WSDataLink(void);
	~WSDataLink(void);

protected:
	virtual void regDatas();

public:
	CString name;   // 工作面名称
	bool hasRisk;   // 是否有突出危险
	double iz;      // 进风巷道标高
	double oz;      // 回风巷道标高
	bool down;      // 是否下行通风
};
