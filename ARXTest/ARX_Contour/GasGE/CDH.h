#pragma once

#include "GasGeologyGE.h"

// ¼ûÃº×ê¿×(CoalDrillHole)
class GASGE_EXPORT_API CDH : public GasGeologyGE
{
public:
	ACRX_DECLARE_MEMBERS(CDH) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	CDH();
	CDH(const AcGePoint3d& insertPt);
};

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CDH)
#endif
