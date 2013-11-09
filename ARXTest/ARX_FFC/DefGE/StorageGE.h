#pragma once

#include "../MineGE/LinkedGE.h"
#include "dlimexp.h"

// 3������ͼԪ�����ݶ�������ͬ�ģ���Ӧ�����Ƕ��ƵĿ��ӻ�Ч��
// ��Ӧͬ������µĻ������
// �������ֻ�ǿ��ӻ�Ч���ĸ���һ������, �����ݺ�Tunnel����һ����
// ��˶�ֱ�Ӵ�StorageGE����������, ��Ϊ"�����־ͼԪ"

// ����ͼԪ����(������)
class DEFGE_EXPORT_API StorageGE : public LinkedGE {

public:
	ACRX_DECLARE_MEMBERS(StorageGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	virtual ~StorageGE();

protected:
	StorageGE();
	StorageGE(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
} ;

// ��ҩ��ͼԪ
class DEFGE_EXPORT_API PowderStorage : public StorageGE {

public:
	ACRX_DECLARE_MEMBERS(PowderStorage) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	PowderStorage();
	PowderStorage(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
	virtual ~PowderStorage();
} ;

// ��������ͼԪ
class DEFGE_EXPORT_API MachineRoom : public StorageGE {

public:
	ACRX_DECLARE_MEMBERS(MachineRoom) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	MachineRoom();
	MachineRoom(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
	virtual ~MachineRoom();
} ;

// �����ͼԪ
class DEFGE_EXPORT_API ChargeRoom : public StorageGE {

public:
	ACRX_DECLARE_MEMBERS(ChargeRoom) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	ChargeRoom();
	ChargeRoom(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
	virtual ~ChargeRoom();
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(StorageGE)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PowderStorage)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MachineRoom)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ChargeRoom)
#endif
