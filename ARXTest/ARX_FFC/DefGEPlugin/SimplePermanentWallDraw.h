#pragma once

#include "SimpleWallDraw.h"

// ���õ���ǽ
class SimplePermanentWallDraw : public SimpleWallDraw {

public:
	ACRX_DECLARE_MEMBERS(SimplePermanentWallDraw) ;

public:
	SimplePermanentWallDraw () ;
	virtual ~SimplePermanentWallDraw () ;

	virtual void setAllExtraParamsToDefault();
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimplePermanentWallDraw)
#endif
