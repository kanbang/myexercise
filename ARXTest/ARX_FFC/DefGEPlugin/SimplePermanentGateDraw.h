#pragma once

#include "SimpleGateDraw.h"

// ���÷���
class SimplePermanentGateDraw : public SimpleGateDraw {

public:
	ACRX_DECLARE_MEMBERS(SimplePermanentGateDraw) ;

public:
	SimplePermanentGateDraw () ;
	virtual ~SimplePermanentGateDraw () ;
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimplePermanentGateDraw)
#endif
