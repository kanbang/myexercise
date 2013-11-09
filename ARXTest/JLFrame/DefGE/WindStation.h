#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// ���վ--������ͼԪ
class DEFGE_EXPORT_API WindStation : public DirectionTagGE
{
public:
	ACRX_DECLARE_MEMBERS(WindStation) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	WindStation();
	WindStation(const AcGePoint3d& insertPt, double angle);
};

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(WindStation)
#endif
