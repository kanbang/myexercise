#pragma once

#include "SimpleCasementDraw.h"

// ǽ���細
class SimpleWallCasementDraw : public SimpleCasementDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleWallCasementDraw) ;

public:
	SimpleWallCasementDraw () ;
	virtual ~SimpleWallCasementDraw () ;

	virtual void setAllExtraParamsToDefault();
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleWallCasementDraw)
#endif
