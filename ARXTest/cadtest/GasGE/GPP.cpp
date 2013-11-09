#include "StdAfx.h"
#include "GPP.h"

Adesk::UInt32 GPP::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    GPP, PointGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ��˹ѹ����, DEFGEAPP
)

GPP::GPP() : PointGE()
{

}

GPP::GPP( const AcGePoint3d& insertPt ) : PointGE( insertPt )
{

}
