#pragma once

#include "../MineGE/DataLink.h"

// 联络巷名称数据
class TunnelNameDataLink : public DataLink
{
public:
	TunnelNameDataLink(void);
	~TunnelNameDataLink(void);

protected:
	virtual void regDatas();

public:
	CString name;
};
