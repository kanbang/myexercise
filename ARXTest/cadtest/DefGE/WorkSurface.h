#pragma once

#include "../MineGE/LinkedGE.h"
#include "dlimexp.h"

// ������(Ŀǰ�����Tunnel�Ķ�����һ����)
class DEFGE_EXPORT_API WorkSurface : public LinkedGE 
{
public:
	ACRX_DECLARE_MEMBERS(WorkSurface) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	WorkSurface () ;
	WorkSurface (const AcGePoint3d& startPt, const AcGePoint3d& endPt) ;
	virtual ~WorkSurface () ;
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(WorkSurface)
#endif
