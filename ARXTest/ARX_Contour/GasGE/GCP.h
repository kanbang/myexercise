#pragma once

#include "GasGeologyGE.h"

// ÍßË¹º¬Á¿µã
// GCP ==> Gas Content Point
class GASGE_EXPORT_API GCP : public GasGeologyGE
{
public:
	ACRX_DECLARE_MEMBERS(GCP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	GCP();
	GCP(const AcGePoint3d& insertPt);
};

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GCP)
#endif
