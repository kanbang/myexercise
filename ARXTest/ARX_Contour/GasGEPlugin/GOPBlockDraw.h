#pragma once

#include "../MineGEDraw/BlockDraw.h"

class GOPBlockDraw : public BlockDraw 
{
public:
	ACRX_DECLARE_MEMBERS(GOPBlockDraw) ;

public:
	virtual void regPropertyDataNames(AcStringArray& names) const;
	virtual void readPropertyDataFromGE(const AcStringArray& values);

public:
	GOPBlockDraw();
};

#ifdef GASGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GOPBlockDraw)
#endif
