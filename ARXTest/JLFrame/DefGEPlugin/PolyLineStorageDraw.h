#pragma once

#include "PolyLineStorageDrawBase.h"

// ��ҩ��
class PolyLinePowderStorageDraw : public PolyLineStorageDrawBase
{
public:
	ACRX_DECLARE_MEMBERS(PolyLinePowderStorageDraw);

public:
	PolyLinePowderStorageDraw();
};

// ��������
class PolyLineMachineRoomDraw : public PolyLineStorageDrawBase
{
public:
	ACRX_DECLARE_MEMBERS(PolyLineMachineRoomDraw);

public:
	PolyLineMachineRoomDraw();
};


// �����
class PolyLineChargeRoomDraw : public PolyLineStorageDrawBase
{
public:
	ACRX_DECLARE_MEMBERS(PolyLineChargeRoomDraw);

public:
	PolyLineChargeRoomDraw();
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PolyLinePowderStorageDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PolyLineMachineRoomDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PolyLineChargeRoomDraw)
#endif