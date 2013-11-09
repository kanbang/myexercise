#include "StdAfx.h"
#include "Casement.h"

Adesk::UInt32 Casement::kCurrentVersionNumber = 1 ;
Adesk::UInt32 WallCasement::kCurrentVersionNumber = 1 ;
Adesk::UInt32 PermanentCasement::kCurrentVersionNumber = 1 ;
Adesk::UInt32 TemporaryCasement::kCurrentVersionNumber = 1 ;

ACRX_NO_CONS_DEFINE_MEMBERS( Casement, DirectionTagGE )

ACRX_DXF_DEFINE_MEMBERS (
    WallCasement, Casement,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ǽ���細, DEFGEAPP
)

ACRX_DXF_DEFINE_MEMBERS (
    PermanentCasement, Casement,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ���õ��細, DEFGEAPP
)

ACRX_DXF_DEFINE_MEMBERS (
    TemporaryCasement, Casement,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ��ʱ���細, DEFGEAPP
)

Casement::Casement() : DirectionTagGE()
{

}

Casement::Casement( const AcGePoint3d& insertPt, double angle ): DirectionTagGE( insertPt, angle )
{

}

WallCasement::WallCasement () : Casement ()
{
}

WallCasement::WallCasement( const AcGePoint3d& insertPt, double angle ) : Casement( insertPt, angle )
{

}

PermanentCasement::PermanentCasement () : Casement ()
{
}

PermanentCasement::PermanentCasement( const AcGePoint3d& insertPt, double angle ) : Casement( insertPt, angle )
{

}

TemporaryCasement::TemporaryCasement () : Casement ()
{
}

TemporaryCasement::TemporaryCasement( const AcGePoint3d& insertPt, double angle ) : Casement( insertPt, angle )
{

}