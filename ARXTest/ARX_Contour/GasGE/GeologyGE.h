#pragma once

#include "GasGE.h"

// 地质图元基类
class GASGE_EXPORT_API GeologyGE : public GasGE
{
public:
	ACRX_DECLARE_MEMBERS(GeologyGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

protected:
	GeologyGE();
};

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GeologyGE)
#endif
