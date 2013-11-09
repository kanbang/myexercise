#pragma once

#include "../MineGE/MineGE.h"

#include "dlimexp.h"

// ÍøÂçÍ¼Í¼Ôª»ùÀà
class VNGGE_DLLIMPEXP VNG_GE : public MineGE 
{
public:
	ACRX_DECLARE_MEMBERS(VNG_GE) ;

public:
	virtual ~VNG_GE () ;

protected:
	VNG_GE () ;
} ;

#ifdef VNGGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_GE)
#endif
