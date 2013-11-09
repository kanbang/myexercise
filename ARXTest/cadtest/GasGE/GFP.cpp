#include "StdAfx.h"
#include "GFP.h"

Adesk::UInt32 GFP::kCurrentVersionNumber = 1 ;
Adesk::UInt32 WGFP::kCurrentVersionNumber = 1 ;
Adesk::UInt32 TGFP::kCurrentVersionNumber = 1 ;

ACRX_NO_CONS_DEFINE_MEMBERS( GFP, PointGE )

ACRX_DXF_DEFINE_MEMBERS (
    WGFP, GFP,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    �زɹ�������˹ӿ������, DEFGEAPP
)

ACRX_DXF_DEFINE_MEMBERS (
    TGFP, GFP,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ��������������˹ӿ������, DEFGEAPP
)

GFP::GFP() : PointGE()
{

}

GFP::GFP( const AcGePoint3d& insertPt ) : PointGE( insertPt )
{

}

WGFP::WGFP() : GFP()
{

}

WGFP::WGFP( const AcGePoint3d& insertPt ) : GFP( insertPt )
{

}

TGFP::TGFP() : GFP()
{

}

TGFP::TGFP( const AcGePoint3d& insertPt ) : GFP( insertPt )
{

}