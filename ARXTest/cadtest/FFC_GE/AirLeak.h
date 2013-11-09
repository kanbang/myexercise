#pragma once

#include "dlimexp.h"
#include "PointGE.h"

// ������--©��Դ��
class FFC_GE_EXPORT_API AirLeak : public PointGE 
{
public:
	ACRX_DECLARE_MEMBERS(AirLeak) ;

protected:
	AirLeak();
	AirLeak(const AcGePoint3d& pt);
} ;

// ©��Դ
class FFC_GE_EXPORT_API SourceAirLeak : public AirLeak 
{
public:
	ACRX_DECLARE_MEMBERS(SourceAirLeak) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	SourceAirLeak();
	SourceAirLeak(const AcGePoint3d& pt);
} ;

// ©���
class FFC_GE_EXPORT_API SinkAirLeak : public AirLeak 
{
public:
	ACRX_DECLARE_MEMBERS(SinkAirLeak) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	SinkAirLeak();
	SinkAirLeak(const AcGePoint3d& pt);
} ;

#ifdef FFC_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(AirLeak)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SourceAirLeak)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SinkAirLeak)
#endif
