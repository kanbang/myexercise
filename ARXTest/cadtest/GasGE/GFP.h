#pragma once

#include "PointGE.h"

// ��˹ӿ�����������
// GFP ==> Gas Flow Point
class GASGE_EXPORT_API GFP : public PointGE
{
public:
	ACRX_DECLARE_MEMBERS(GFP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

protected:
	GFP();
	GFP(const AcGePoint3d& insertPt);
};

// �زɹ�������˹ӿ������(����������˹ӿ�����������˹ӿ����)
class GASGE_EXPORT_API WGFP : public GFP
{
public:
	ACRX_DECLARE_MEMBERS(WGFP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	WGFP();
	WGFP(const AcGePoint3d& insertPt);
};

// ��������������˹ӿ������
class GASGE_EXPORT_API TGFP : public GFP
{
public:
	ACRX_DECLARE_MEMBERS(TGFP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	TGFP();
	TGFP(const AcGePoint3d& insertPt);
};

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GFP)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(WGFP)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TGFP)
#endif
