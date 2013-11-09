#pragma once

#include "GasGeologyGE.h"

// 动力现象点
// PPP ==> Power Phenomenon Point
class GASGE_EXPORT_API PPP : public GasGeologyGE
{
public:
	ACRX_DECLARE_MEMBERS(PPP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	PPP();
	PPP(const AcGePoint3d& insertPt);
};

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PPP)
#endif
