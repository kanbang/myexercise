#pragma once

#include "../MineGEDraw/BlockDraw.h"

class CompassBlockDraw : public BlockDraw {

public:
	ACRX_DECLARE_MEMBERS(CompassBlockDraw) ;

public:
	CompassBlockDraw();
	virtual ~CompassBlockDraw();

	virtual void regPropertyDataNames(AcStringArray& names) const;
	virtual void readPropertyDataFromGE(const AcStringArray& values);

} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CompassBlockDraw)
#endif
