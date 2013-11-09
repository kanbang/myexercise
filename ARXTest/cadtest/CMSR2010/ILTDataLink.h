#pragma once

#include "TunnelNameDataLink.h"

class ILTDataLink : public TunnelNameDataLink
{
public:
	ILTDataLink(void);
	~ILTDataLink(void);

public:
	CString inEdgeName;  // 主进风巷
	CString outEdgeName; // 主回风巷
	bool hasWall;        // 是否有构筑物
};
