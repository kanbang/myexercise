#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

// 瓦斯相关的图元基类
class GASGE_EXPORT_API GasGE : public MineGE
{
public:
	ACRX_DECLARE_MEMBERS(GasGE) ;

protected:
	GasGE(void);
};

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GasGE)
#endif
