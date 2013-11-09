#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// ���ڷ細��ǽ���細�����÷���ǽ���細����ʱ����ǽ���細

// ���細������
class DEFGE_EXPORT_API Casement : public DirectionTagGE
{
public:
	ACRX_DECLARE_MEMBERS(Casement) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

protected:
	Casement();
	Casement(const AcGePoint3d& insertPt, double angle);
};

// ǽ���細
class DEFGE_EXPORT_API WallCasement : public Casement {

public:
	ACRX_DECLARE_MEMBERS(WallCasement) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	WallCasement() ;
	WallCasement(const AcGePoint3d& insertPt, double angle);
};

// ���÷���ǽ���細
class DEFGE_EXPORT_API PermanentCasement : public Casement {

public:
	ACRX_DECLARE_MEMBERS(PermanentCasement) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	PermanentCasement() ;
	PermanentCasement(const AcGePoint3d& insertPt, double angle);
};

// ��ʱ����ǽ���細
class DEFGE_EXPORT_API TemporaryCasement : public Casement {

public:
	ACRX_DECLARE_MEMBERS(TemporaryCasement) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	TemporaryCasement() ;
	TemporaryCasement(const AcGePoint3d& insertPt, double angle);
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Casement)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(WallCasement)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PermanentCasement)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TemporaryCasement)
#endif
