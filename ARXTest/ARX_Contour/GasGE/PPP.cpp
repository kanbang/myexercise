#include "StdAfx.h"
#include "PPP.h"

Adesk::UInt32 PPP::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    PPP, GasGeologyGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    动力现象点, GASGEAPP
)

PPP::PPP() : GasGeologyGE()
{

}

PPP::PPP( const AcGePoint3d& insertPt ) : GasGeologyGE( insertPt )
{

}
