#pragma once

#include "dlimexp.h"

#define PI 3.1415926535897932384626433832795

// 等值图抽象基类
class CONTOURGE_DLLIMPEXP ContourGE  : public AcDbEntity
{
public:
	ACRX_DECLARE_MEMBERS(ContourGE) ;

protected:
	ContourGE(void);
};

#ifdef CONTOURGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ContourGE)
#endif
