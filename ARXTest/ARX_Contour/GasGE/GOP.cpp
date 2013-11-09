#include "StdAfx.h"
#include "GOP.h"

Adesk::UInt32 GOP::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    GOP, GasGeologyGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ú����˹ͻ����, GASGEAPP
)

GOP::GOP() : GasGeologyGE()
{

}

GOP::GOP( const AcGePoint3d& insertPt ): GasGeologyGE( insertPt )
{

}