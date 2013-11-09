#pragma once

#include "GasGeologyGE.h"

// 瓦斯突出点抽象类
// GOP ==> Gas Outburst Point
class GASGE_EXPORT_API GOP : public GasGeologyGE
{
public:
	ACRX_DECLARE_MEMBERS(GOP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	GOP();
	GOP(const AcGePoint3d& insertPt);
};

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GOP)
#endif
