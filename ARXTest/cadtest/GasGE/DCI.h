#pragma once

#include "PointGE.h"

// ͻ��Σ����Ԥ��ָ��
// ������(ûʲôʵ�����壬����Ϊ������)
// DCI ==> Dangerous Calculate Index
class GASGE_EXPORT_API DCI : public PointGE
{
public:
	ACRX_DECLARE_MEMBERS(DCI) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

protected:
	DCI();
	DCI(const AcGePoint3d& insertPt);
};

// ú������ͻ��Σ����Ԥ��ָ��ֵ
// SDCI ==> Coal Seam Dangerous Calculate Index
class GASGE_EXPORT_API CSDCI : public DCI
{
public:
	ACRX_DECLARE_MEMBERS(CSDCI) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	CSDCI();
	CSDCI(const AcGePoint3d& insertPt);
};

// ������ͻ��Σ����Ԥ��ָ��ֵ1
// WDCI1 ==> Worksurface Dangerous Calculate Index 1
class GASGE_EXPORT_API WDCI1 : public DCI
{
public:
	ACRX_DECLARE_MEMBERS(WDCI1) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	WDCI1();
	WDCI1(const AcGePoint3d& insertPt);
};

// ������ͻ��Σ����Ԥ��ָ��ֵ2
// WDCI2 ==> Worksurface Dangerous Calculate Index 2
class GASGE_EXPORT_API WDCI2 : public DCI
{
public:
	ACRX_DECLARE_MEMBERS(WDCI2) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	WDCI2();
	WDCI2(const AcGePoint3d& insertPt);
};

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DCI)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CSDCI)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(WDCI1)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(WDCI2)
#endif
