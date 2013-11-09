#include "StdAfx.h"
#include "DoubleStorageDraw.h"

ACRX_CONS_DEFINE_MEMBERS ( DoublePowderStorageDraw, DoubleStorageDrawBase, 1 )
ACRX_CONS_DEFINE_MEMBERS ( DoubleMachineRoomDraw, DoubleStorageDrawBase, 1 )
ACRX_CONS_DEFINE_MEMBERS ( DoubleChargeRoomDraw, DoubleStorageDrawBase, 1 )

DoublePowderStorageDraw::DoublePowderStorageDraw( void ) : DoubleStorageDrawBase( _T( "��ը���Ͽ�" ) )
{
}

DoubleMachineRoomDraw::DoubleMachineRoomDraw( void ) : DoubleStorageDrawBase( _T( "��������" ) )
{
}

DoubleChargeRoomDraw::DoubleChargeRoomDraw( void ) : DoubleStorageDrawBase( _T( "�����" ) )
{
}
