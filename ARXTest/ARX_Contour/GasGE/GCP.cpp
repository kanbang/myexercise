#include "StdAfx.h"
#include "GCP.h"

Adesk::UInt32 GCP::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    GCP, GasGeologyGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ÍßË¹º¬Á¿µã, GASGEAPP
)

GCP::GCP() : GasGeologyGE()
{

}

GCP::GCP( const AcGePoint3d& insertPt ) : GasGeologyGE( insertPt )
{

}
