#include "StdAfx.h"
#include "FFC_GEPlugin.h"

// ɳ���ܱ�ǽ
#include "SimpleSandWallDraw.h"

// �ܱ�
#include "SingleObturationDraw.h"
#include "RectObturationDraw.h"

// �ɿ���
#include "SimpleGoafDraw.h"

// ע����
#include "SphereNitrogenPipeDraw.h"

// ��˹���
#include "DoubleGasPipeDraw.h"
#include "SingleGasPipeDraw.h"

// ©��Դ��
#include "SimpleAirLeakDraw.h"

void FFC_GEPlugin::initialize( MineGEDrawManager* pGEDrawManager )
{
    BEGIN_REGISTER_MINEGE_DRAW( MineGEDrawManager, pGEDrawManager )
    // ɳ����ǽ
    REGISTER_MINEGE_DRAW( SandWall, SimpleSandWallDraw )

    // �ܱ�
    REGISTER_MINEGE_DRAW( Obturation, RectObturationDraw )
    REGISTER_MINEGE_DRAW( Obturation, SingleObturationDraw )

    // �ɿ���
    REGISTER_MINEGE_DRAW( Goaf, SimpleGoafDraw )

    // ע����
    REGISTER_MINEGE_DRAW( NitrogenPipe, SphereNitrogenPipeDraw )

    // ��˹���
    REGISTER_MINEGE_DRAW( GasPipe, SingleGasPipeDraw )
    REGISTER_MINEGE_DRAW( GasPipe, DoubleGasPipeDraw )

    // ©��Դ��
    REGISTER_MINEGE_DRAW( SourceAirLeak, SimpleSourceAirLeakDraw )
    REGISTER_MINEGE_DRAW( SinkAirLeak, SimpleSinkAirLeakDraw )

    END_REGISTER_MINEGE_DRAW
}

void FFC_GEPlugin::unInitialize( MineGEDrawManager* pGEDrawManager )
{
    BEGIN_UNREGISTER_MINEGE_DRAW( MineGEDrawManager, pGEDrawManager )

    // ɳ����ǽ
    UNREGISTER_MINEGE_DRAW( SandWall, SimpleSandWallDraw )

    // �ܱ�
    UNREGISTER_MINEGE_DRAW( Obturation, RectObturationDraw )
    UNREGISTER_MINEGE_DRAW( Obturation, SingleObturationDraw )

    // �ɿ���
    UNREGISTER_MINEGE_DRAW( Goaf, SimpleGoafDraw )

    // ע����
    UNREGISTER_MINEGE_DRAW( NitrogenPipe, SphereNitrogenPipeDraw )

    // ��˹���
    UNREGISTER_MINEGE_DRAW( GasPipe, SingleGasPipeDraw )
    UNREGISTER_MINEGE_DRAW( GasPipe, DoubleGasPipeDraw )

    // ©��Դ��
    UNREGISTER_MINEGE_DRAW( SourceAirLeak, SimpleSourceAirLeakDraw )
    UNREGISTER_MINEGE_DRAW( SinkAirLeak, SimpleSinkAirLeakDraw )

    END_UNREGISTER_MINEGE_DRAW
}

// ���������������ʵ�ֲ�����
MINEGE_PLUGIN_CREATE_FUNC_IMPL( FFC_GEPlugin )

//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(WorkSurface, DoubleWorkSurfaceDraw, DoubleWorkSurfaceDraw_Jig)

// draw��������ʵ�ֺ�(ɾ���ˣ���Ϊ������ȷ��draw�Ĺ��췽ʽ��Ŀǰ��draw��ͨ��"����+ARX RTTI"��������)
