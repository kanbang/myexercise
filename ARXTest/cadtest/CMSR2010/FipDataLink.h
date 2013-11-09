#pragma once

#include "TunnelNameDataLink.h"

class FipDataLink : public TunnelNameDataLink
{
public:
	FipDataLink(void);
	~FipDataLink(void);

public:
	AcStringArray p1;
	AcStringArray p2;
	bool includeAir;
};
