#pragma once

#include "SimpleCasementDraw.h"

// ���÷���ǽ���細
class SimplePermanentCasementDraw : public SimpleCasementDraw {

public:
	ACRX_DECLARE_MEMBERS(SimplePermanentCasementDraw) ;

public:
	SimplePermanentCasementDraw () ;
	virtual ~SimplePermanentCasementDraw () ;

	virtual void setAllExtraParamsToDefault();
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimplePermanentCasementDraw)
#endif
