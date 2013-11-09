#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// ���ţ����÷��ţ���ʱ���ţ�˫�����

// ���ų�����
class DEFGE_EXPORT_API Gate : public DirectionTagGE
{
public:
	ACRX_DECLARE_MEMBERS(Gate) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

protected:
	Gate();
	Gate(const AcGePoint3d& insertPt, double angle);
};

// ���÷���
class DEFGE_EXPORT_API PermanentGate : public Gate {

public:
	ACRX_DECLARE_MEMBERS(PermanentGate) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	PermanentGate() ;
	PermanentGate(const AcGePoint3d& insertPt, double angle);
};

// ��ʱ����
class DEFGE_EXPORT_API TemporaryGate : public Gate {

public:
	ACRX_DECLARE_MEMBERS(TemporaryGate) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	TemporaryGate() ;
	TemporaryGate(const AcGePoint3d& insertPt, double angle);
} ;

// ˫�����(ò�ƾ�����Ϸ���???)
class DEFGE_EXPORT_API DoubleGate : public Gate {

public:
	ACRX_DECLARE_MEMBERS(DoubleGate) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	DoubleGate();
	DoubleGate(const AcGePoint3d& insertPt, double angle);
};

// ƽ�����(ò��Ҳ����Ϸ���???)
class DEFGE_EXPORT_API BalanceGate : public Gate {

public:
	ACRX_DECLARE_MEMBERS(BalanceGate) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	BalanceGate();
	BalanceGate(const AcGePoint3d& insertPt, double angle);
};

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Gate)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PermanentGate)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TemporaryGate)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleGate)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(BalanceGate)
#endif
