#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// ���������
class DEFGE_EXPORT_API Fan : public DirectionTagGE
{
public:
	ACRX_DECLARE_MEMBERS(Fan) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

protected:
	Fan() ;
	Fan(const AcGePoint3d& insertPt, double angle);
};

// ��Ҫͨ���
class DEFGE_EXPORT_API MainFan : public Fan {

public:
	ACRX_DECLARE_MEMBERS(MainFan) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	MainFan () ;
	MainFan(const AcGePoint3d& insertPt, double angle);
} ;

// �ֲ��ȷ��
class DEFGE_EXPORT_API LocalFan : public Fan {

public:
	ACRX_DECLARE_MEMBERS(LocalFan) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	LocalFan() ;
	LocalFan(const AcGePoint3d& insertPt, double angle);
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Fan)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MainFan)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(LocalFan)
#endif
