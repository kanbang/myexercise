#include "StdAfx.h"
#include "FFC_GEPlugin.h"

// 沙土密闭墙
#include "SimpleSandWallDraw.h"

// 密闭
#include "SingleObturationDraw.h"
#include "RectObturationDraw.h"

// 采空区
#include "SimpleGoafDraw.h"

// 注氮孔
#include "SphereNitrogenPipeDraw.h"

// 瓦斯钻孔
#include "DoubleGasPipeDraw.h"
#include "SingleGasPipeDraw.h"

// 漏风源汇
#include "SimpleAirLeakDraw.h"

void FFC_GEPlugin::initialize( MineGEDrawManager* pGEDrawManager )
{
    BEGIN_REGISTER_MINEGE_DRAW( MineGEDrawManager, pGEDrawManager )
    // 沙土闭墙
    REGISTER_MINEGE_DRAW( SandWall, SimpleSandWallDraw )

    // 密闭
    REGISTER_MINEGE_DRAW( Obturation, RectObturationDraw )
    REGISTER_MINEGE_DRAW( Obturation, SingleObturationDraw )

    // 采空区
    REGISTER_MINEGE_DRAW( Goaf, SimpleGoafDraw )

    // 注氮孔
    REGISTER_MINEGE_DRAW( NitrogenPipe, SphereNitrogenPipeDraw )

    // 瓦斯钻孔
    REGISTER_MINEGE_DRAW( GasPipe, SingleGasPipeDraw )
    REGISTER_MINEGE_DRAW( GasPipe, DoubleGasPipeDraw )

    // 漏风源汇
    REGISTER_MINEGE_DRAW( SourceAirLeak, SimpleSourceAirLeakDraw )
    REGISTER_MINEGE_DRAW( SinkAirLeak, SimpleSinkAirLeakDraw )

    END_REGISTER_MINEGE_DRAW
}

void FFC_GEPlugin::unInitialize( MineGEDrawManager* pGEDrawManager )
{
    BEGIN_UNREGISTER_MINEGE_DRAW( MineGEDrawManager, pGEDrawManager )

    // 沙土闭墙
    UNREGISTER_MINEGE_DRAW( SandWall, SimpleSandWallDraw )

    // 密闭
    UNREGISTER_MINEGE_DRAW( Obturation, RectObturationDraw )
    UNREGISTER_MINEGE_DRAW( Obturation, SingleObturationDraw )

    // 采空区
    UNREGISTER_MINEGE_DRAW( Goaf, SimpleGoafDraw )

    // 注氮孔
    UNREGISTER_MINEGE_DRAW( NitrogenPipe, SphereNitrogenPipeDraw )

    // 瓦斯钻孔
    UNREGISTER_MINEGE_DRAW( GasPipe, SingleGasPipeDraw )
    UNREGISTER_MINEGE_DRAW( GasPipe, DoubleGasPipeDraw )

    // 漏风源汇
    UNREGISTER_MINEGE_DRAW( SourceAirLeak, SimpleSourceAirLeakDraw )
    UNREGISTER_MINEGE_DRAW( SinkAirLeak, SimpleSinkAirLeakDraw )

    END_UNREGISTER_MINEGE_DRAW
}

// 插件创建函数定义实现并导出
MINEGE_PLUGIN_CREATE_FUNC_IMPL( FFC_GEPlugin )

//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(WorkSurface, DoubleWorkSurfaceDraw, DoubleWorkSurfaceDraw_Jig)

// draw创建函数实现宏(删除了，因为还不能确定draw的构造方式，目前的draw是通过"名称+ARX RTTI"来创建的)
