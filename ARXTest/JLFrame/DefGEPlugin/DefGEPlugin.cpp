#include "StdAfx.h"
#include "DefGEPlugin.h"

// ˫�����
#include "DoubleTunnelDraw.h"

// �������
#include "SingleTunnelDraw.h"

// ˫�������
#include "DoubleArcTunnelDraw.h"

// ��������
#include "PolyLineTunnelDraw.h"

// ����ͷ
#include "SimpelDirectionDraw.h"

// ����߼�ͷ
#include "PolyLineDirectionDraw.h"

//// Բ�νڵ�(ʮ��)
//#include "CircularJointDraw.h"
//
//// Բ�νڵ�(���)
//#include "CircularJointDraw2.h"
//
//// �սڵ�
//#include "NullJointDraw.h"

// �򵥷��
#include "SimpleMainFanDraw.h"
#include "SimpleLocalFanDraw.h"

// �򵥷�Ͳ
#include "SimpleChimneyDraw.h"

// ���÷���
#include "SimplePermanentGateDraw.h"
// ��ʱ����
#include "SimpleTemporaryGateDraw.h"
// ˫�����
#include "SimpleDoubleGateDraw.h"
// ƽ�����
#include "SimpleBalanceGateDraw.h"

// ���õ���ǽ
#include "SimplePermanentWallDraw.h"
// ��ʱ����ǽ
#include "SimpleTemporaryWallDraw.h"

// ǽ���ڷ細
#include "SimpleWallCasementDraw.h"
// ���÷��ŵ��ڷ細
#include "SimplePermanentCasementDraw.h"
// ��ʱ���ŵ��ڷ細
#include "SimpleTemporaryCasementDraw.h"

// ���վ
#include "SimpleWindStationDraw.h"

// ������
#include "DoubleWorkSurfaceDraw.h"
#include "PolyLineWorkSurfaceDraw.h"

// ���������
#include "DoubleTTunnelDraw.h"
#include "PolyLineTTunnelDraw.h"

// ��ҩ�⡢����ҡ���������
#include "DoubleStorageDraw.h"
#include "PolyLineStorageDraw.h"

// ��Ͳ
//#include "DoubleShaftDraw.h"

// ָ����
//#include "CompassBlockDraw.h"

// ����
#include "DoubleWindBridgeDraw.h"

// CircularJointDraw���ӻ�Ч����Jigʵ��
//#include "CircularJointDraw_Jig.h"

// SingleTunnelDraw���ӻ�Ч����Jigʵ��
//#include "SingleTunnelDraw_Jig.h"

// DoubleTunnelDraw���ӻ�Ч����Jigʵ��
//#include "DoubleTunnelDraw_Jig.h"

// DoubleArcTunnelDraw���ӻ�Ч����Jigʵ��
//#include "SingleArcTunnelDraw_Jig.h"

// DoubleArcTunnelDraw���ӻ�Ч����jigʵ��
//#include "DoubleArcTunnelDraw_Jig.h"

// DoubleWorkSurfaceDraw���ӻ�Ч����jigʵ��
//#include "DoubleWorkSurfaceDraw_Jig.h"

void DefGEPlugin::initialize( MineGEDrawManager* pGEDrawManager )
{
    BEGIN_REGISTER_MINEGE_DRAW( MineGEDrawManager, pGEDrawManager )
    // ���
    REGISTER_MINEGE_DRAW( Tunnel, DoubleTunnelDraw )
    REGISTER_MINEGE_DRAW( Tunnel, PolyLineTunnelDraw )
    REGISTER_MINEGE_DRAW( Tunnel, SingleTunnelDraw )
    //REGISTER_MINEGE_DRAW_JIG(Tunnel, DoubleTunnelDraw, DoubleTunnelDraw_Jig)
    //REGISTER_MINEGE_DRAW_JIG(Tunnel, SingleTunnelDraw, SingleTunnelDraw_Jig)

    // �������
    REGISTER_MINEGE_DRAW( ArcTunnel, DoubleArcTunnelDraw )
    //REGISTER_MINEGE_DRAW_JIG(ArcTunnel, DoubleArcTunnelDraw, DoubleArcTunnelDraw_Jig)
    //REGISTER_MINEGE_DRAW_JIG(ArcTunnel, DoubleArcTunnelDraw, SingleArcTunnelDraw_Jig)

    // �زɹ�����
    REGISTER_MINEGE_DRAW( WorkSurface, DoubleWorkSurfaceDraw )
    REGISTER_MINEGE_DRAW( WorkSurface, PolyLineWorkSurfaceDraw )
    //REGISTER_MINEGE_DRAW_JIG(WorkSurface, DoubleWorkSurfaceDraw, DoubleWorkSurfaceDraw_Jig)

    // ���������
    REGISTER_MINEGE_DRAW( TTunnel, DoubleTTunnelDraw )
    REGISTER_MINEGE_DRAW( TTunnel, PolyLineTTunnelDraw )

    REGISTER_MINEGE_DRAW( WindDirection, PolyLineDirectionDraw )
    REGISTER_MINEGE_DRAW( WindDirection, SimpelDirectionDraw )

    // �ڵ�
    //REGISTER_MINEGE_DRAW(Joint, CircularJointDraw)
    ////REGISTER_MINEGE_DRAW_JIG(Joint, CircularJointDraw, CircularJointDraw_Jig)
    //REGISTER_MINEGE_DRAW(Joint, CircularJointDraw2)
    //REGISTER_MINEGE_DRAW(Joint, NullJointDraw)

    // ���
    REGISTER_MINEGE_DRAW( MainFan, SimpleMainFanDraw )
    REGISTER_MINEGE_DRAW( LocalFan, SimpleLocalFanDraw )

    // ��Ͳ
    REGISTER_MINEGE_DRAW( Chimney, SimpleChimneyDraw )

    // ����
    REGISTER_MINEGE_DRAW( PermanentGate, SimplePermanentGateDraw )
    REGISTER_MINEGE_DRAW( TemporaryGate, SimpleTemporaryGateDraw )
    REGISTER_MINEGE_DRAW( DoubleGate, SimpleDoubleGateDraw )
    REGISTER_MINEGE_DRAW( BalanceGate, SimpleBalanceGateDraw )

    // ����ǽ
    REGISTER_MINEGE_DRAW( PermanentWall, SimplePermanentWallDraw )
    REGISTER_MINEGE_DRAW( TemporaryWall, SimpleTemporaryWallDraw )

    // ���ڷ�ǽ
    REGISTER_MINEGE_DRAW( WallCasement, SimpleWallCasementDraw )
    REGISTER_MINEGE_DRAW( PermanentCasement, SimplePermanentCasementDraw )
    REGISTER_MINEGE_DRAW( TemporaryCasement, SimpleTemporaryCasementDraw )

    // ���վ
    REGISTER_MINEGE_DRAW( WindStation, SimpleWindStationDraw )

    // ����
    REGISTER_MINEGE_DRAW( PowderStorage, DoublePowderStorageDraw )
    REGISTER_MINEGE_DRAW( MachineRoom, DoubleMachineRoomDraw )
    REGISTER_MINEGE_DRAW( ChargeRoom, DoubleChargeRoomDraw )

    REGISTER_MINEGE_DRAW( PowderStorage, PolyLinePowderStorageDraw )
    REGISTER_MINEGE_DRAW( MachineRoom, PolyLineMachineRoomDraw )
    REGISTER_MINEGE_DRAW( ChargeRoom, PolyLineChargeRoomDraw )

    //REGISTER_MINEGE_DRAW(Shaft, DoubleShaftDraw)

    // ָ����
    //REGISTER_MINEGE_DRAW(Compass, CompassBlockDraw)

    // ����
    REGISTER_MINEGE_DRAW( WindBridge, DoubleWindBridgeDraw )

    END_REGISTER_MINEGE_DRAW
}

void DefGEPlugin::unInitialize( MineGEDrawManager* pGEDrawManager )
{
    BEGIN_UNREGISTER_MINEGE_DRAW( MineGEDrawManager, pGEDrawManager )
    // ���
    UNREGISTER_MINEGE_DRAW( Tunnel, DoubleTunnelDraw )
    UNREGISTER_MINEGE_DRAW( Tunnel, PolyLineTunnelDraw )
    UNREGISTER_MINEGE_DRAW( Tunnel, SingleTunnelDraw )

    // �������
    UNREGISTER_MINEGE_DRAW( ArcTunnel, DoubleArcTunnelDraw )
    UNREGISTER_MINEGE_DRAW( ArcTunnel, DoubleArcTunnelDraw )

    // �زɹ�����
    UNREGISTER_MINEGE_DRAW( WorkSurface, DoubleWorkSurfaceDraw )
    UNREGISTER_MINEGE_DRAW( WorkSurface, PolyLineWorkSurfaceDraw )

    // ���������
    UNREGISTER_MINEGE_DRAW( TTunnel, DoubleTTunnelDraw )
    UNREGISTER_MINEGE_DRAW( TTunnel, PolyLineTTunnelDraw )

    // �ڵ�
    /*UNREGISTER_MINEGE_DRAW(Joint, CircularJointDraw)
    UNREGISTER_MINEGE_DRAW(Joint, CircularJointDraw2)
    UNREGISTER_MINEGE_DRAW(Joint, NullJointDraw)*/

    // ��������
    UNREGISTER_MINEGE_DRAW( WindDirection, PolyLineDirectionDraw )
    UNREGISTER_MINEGE_DRAW( WindDirection, SimpelDirectionDraw )

    // ���
    UNREGISTER_MINEGE_DRAW( MainFan, SimpleMainFanDraw )
    UNREGISTER_MINEGE_DRAW( LocalFan, SimpleLocalFanDraw )

    // ��Ͳ
    UNREGISTER_MINEGE_DRAW( Chimney, SimpleChimneyDraw )

    // ����
    UNREGISTER_MINEGE_DRAW( PermanentGate, SimplePermanentGateDraw )
    UNREGISTER_MINEGE_DRAW( TemporaryGate, SimpleTemporaryGateDraw )
    UNREGISTER_MINEGE_DRAW( DoubleGate, SimpleDoubleGateDraw )
    UNREGISTER_MINEGE_DRAW( BalanceGate, SimpleBalanceGateDraw )

    // ����ǽ
    UNREGISTER_MINEGE_DRAW( PermanentWall, SimplePermanentWallDraw )
    UNREGISTER_MINEGE_DRAW( TemporaryWall, SimpleTemporaryWallDraw )

    // ���ڷ�ǽ
    UNREGISTER_MINEGE_DRAW( WallCasement, SimpleWallCasementDraw )
    UNREGISTER_MINEGE_DRAW( PermanentCasement, SimplePermanentCasementDraw )
    UNREGISTER_MINEGE_DRAW( TemporaryCasement, SimpleTemporaryCasementDraw )

    // ���վ
    UNREGISTER_MINEGE_DRAW( WindStation, SimpleWindStationDraw )

    // ����
    UNREGISTER_MINEGE_DRAW( PowderStorage, DoublePowderStorageDraw )
    UNREGISTER_MINEGE_DRAW( MachineRoom, DoubleMachineRoomDraw )
    UNREGISTER_MINEGE_DRAW( ChargeRoom, DoubleChargeRoomDraw )
    UNREGISTER_MINEGE_DRAW( PowderStorage, PolyLinePowderStorageDraw )
    UNREGISTER_MINEGE_DRAW( MachineRoom, PolyLineMachineRoomDraw )
    UNREGISTER_MINEGE_DRAW( ChargeRoom, PolyLineChargeRoomDraw )

    //UNREGISTER_MINEGE_DRAW(Shaft, DoubleShaftDraw)

    // ָ����
    //UNREGISTER_MINEGE_DRAW(Compass, CompassBlockDraw)

    // ����
    UNREGISTER_MINEGE_DRAW( WindBridge, DoubleWindBridgeDraw )

    END_UNREGISTER_MINEGE_DRAW
}

// ���������������ʵ�ֲ�����
MINEGE_PLUGIN_CREATE_FUNC_IMPL( DefGEPlugin )

//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(Joint, CircularJointDraw, CircularJointDraw_Jig)

//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(Tunnel, SingleTunnelDraw, SingleTunnelDraw_Jig)
//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(Tunnel, DoubleTunnelDraw, DoubleTunnelDraw_Jig)
//
//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(ArcTunnel, DoubleArcTunnelDraw, SingleArcTunnelDraw_Jig)
//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(ArcTunnel, DoubleArcTunnelDraw, DoubleArcTunnelDraw_Jig)
//
//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(WorkSurface, DoubleWorkSurfaceDraw, DoubleWorkSurfaceDraw_Jig)

// draw��������ʵ�ֺ�(ɾ���ˣ���Ϊ������ȷ��draw�Ĺ��췽ʽ��Ŀǰ��draw��ͨ��"����+ARX RTTI"��������)