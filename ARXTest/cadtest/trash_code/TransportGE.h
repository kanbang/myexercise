#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// 运输标记类图元
class DEFGE_EXPORT_API TransportGE : public DirectionTagGE {

public:	
	ACRX_DECLARE_MEMBERS(TransportGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	virtual ~TransportGE () ;

protected:
	TransportGE(void);
	TransportGE(const AcGePoint3d& insertPt, double angle);
};

// 胶轮车(防爆柴油机)
class DEFGE_EXPORT_API Diesel : public TransportGE {

public:	
	ACRX_DECLARE_MEMBERS(Diesel) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	Diesel(void);
	Diesel(const AcGePoint3d& insertPt, double angle);
	virtual ~Diesel () ;
};

// 架线电车
class DEFGE_EXPORT_API ElectricWireCar : public TransportGE {

public:	
	ACRX_DECLARE_MEMBERS(ElectricWireCar) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	ElectricWireCar(void);
	ElectricWireCar(const AcGePoint3d& insertPt, double angle);
	virtual ~ElectricWireCar () ;
};

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TransportGE)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Diesel)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ElectricWireCar)
#endif