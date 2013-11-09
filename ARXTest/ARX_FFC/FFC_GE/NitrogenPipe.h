#pragma once

#include "dlimexp.h"
#include "PointGE.h"

// ×¢µª¿×
class FFC_GE_EXPORT_API NitrogenPipe : public PointGE 
{
public:
	ACRX_DECLARE_MEMBERS(NitrogenPipe) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	NitrogenPipe();
	NitrogenPipe(const AcGePoint3d& pt);
	virtual ~NitrogenPipe();
} ;

#ifdef FFC_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(NitrogenPipe)
#endif
