#include "StdAfx.h"
#include "GasGEPlugin.h"

// ��˹ͻ����
#include "GOPBlockDraw.h"
#include "GCPBlockDraw.h"
#include "GPPBlockDraw.h"
#include "PPPBlockDraw.h"
#include "DCIBlockDraw.h"
#include "GFPBlockDraw.h"
#include "CDHBlockDraw.h"

#include "SimpleMineBoundaryDraw.h"

void GasGEPlugin::initialize( MineGEDrawManager* pGEDrawManager )
{
    BEGIN_REGISTER_MINEGE_DRAW( MineGEDrawManager, pGEDrawManager )

    // ��˹ͻ����
    REGISTER_MINEGE_DRAW( GOP, GOPBlockDraw )

    // ��˹������
    REGISTER_MINEGE_DRAW( GCP, GCPBlockDraw )

    // ��˹ѹ����
    REGISTER_MINEGE_DRAW( GPP, GPPBlockDraw )

    // ���������
    REGISTER_MINEGE_DRAW( PPP, PPPBlockDraw )

    // ͻ��Σ����Ԥ��ָ��
    REGISTER_MINEGE_DRAW( CSDCI, CSDCIBlockDraw )
    REGISTER_MINEGE_DRAW( WDCI1, WDCI1BlockDraw )
    REGISTER_MINEGE_DRAW( WDCI2, WDCI2BlockDraw )

    // ��˹ӿ����
    REGISTER_MINEGE_DRAW( WGFP, WGFPBlockDraw )
    REGISTER_MINEGE_DRAW( TGFP, TGFPBlockDraw )

    // ��ú���
    REGISTER_MINEGE_DRAW( CDH, CDHBlockDraw )

    // ����߽�
    REGISTER_MINEGE_DRAW( MineBoundary, SimpleMineBoundaryDraw )

    END_REGISTER_MINEGE_DRAW
}

void GasGEPlugin::unInitialize( MineGEDrawManager* pGEDrawManager )
{
    BEGIN_UNREGISTER_MINEGE_DRAW( MineGEDrawManager, pGEDrawManager )

    // ��˹ͻ����
    UNREGISTER_MINEGE_DRAW( GOP, GOPBlockDraw )

    // ��˹������
    UNREGISTER_MINEGE_DRAW( GCP, GCPBlockDraw )

    // ��˹ѹ����
    UNREGISTER_MINEGE_DRAW( GPP, GPPBlockDraw )

    // ���������
    UNREGISTER_MINEGE_DRAW( PPP, PPPBlockDraw )

    // ͻ��Σ����Ԥ��ָ��
    UNREGISTER_MINEGE_DRAW( CSDCI, CSDCIBlockDraw )
    UNREGISTER_MINEGE_DRAW( WDCI1, WDCI1BlockDraw )
    UNREGISTER_MINEGE_DRAW( WDCI2, WDCI2BlockDraw )

    // ��˹ӿ����
    UNREGISTER_MINEGE_DRAW( WGFP, WGFPBlockDraw )
    UNREGISTER_MINEGE_DRAW( TGFP, TGFPBlockDraw )

    // ��ú���
    UNREGISTER_MINEGE_DRAW( CDH, CDHBlockDraw )

    // ����߽�
    UNREGISTER_MINEGE_DRAW( MineBoundary, SimpleMineBoundaryDraw )

    END_UNREGISTER_MINEGE_DRAW
}

// ���������������ʵ�ֲ�����
MINEGE_PLUGIN_CREATE_FUNC_IMPL( GasGEPlugin )

// draw��������ʵ�ֺ�(ɾ���ˣ���Ϊ������ȷ��draw�Ĺ��췽ʽ��Ŀǰ��draw��ͨ��"����+ARX RTTI"��������)