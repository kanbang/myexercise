#include "StdAfx.h"
#include "Gate.h"

Adesk::UInt32 Gate::kCurrentVersionNumber = 1 ;
Adesk::UInt32 PermanentGate::kCurrentVersionNumber = 1 ;
Adesk::UInt32 TemporaryGate::kCurrentVersionNumber = 1 ;
Adesk::UInt32 DoubleGate::kCurrentVersionNumber = 1 ;
Adesk::UInt32 BalanceGate::kCurrentVersionNumber = 1 ;

ACRX_NO_CONS_DEFINE_MEMBERS( Gate, DirectionTagGE )

ACRX_DXF_DEFINE_MEMBERS (
    PermanentGate, Gate,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ���÷���, DEFGEAPP
)

ACRX_DXF_DEFINE_MEMBERS (
    TemporaryGate, Gate,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ��ʱ����, DEFGEAPP
)

ACRX_DXF_DEFINE_MEMBERS (
    DoubleGate, Gate,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ˫�����, DEFGEAPP
)

ACRX_DXF_DEFINE_MEMBERS (
    BalanceGate, Gate,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ƽ�����, DEFGEAPP
)

Gate::Gate() : DirectionTagGE()
{

}

Gate::Gate( const AcGePoint3d& insertPt, double angle ): DirectionTagGE( insertPt, angle )
{

}

PermanentGate::PermanentGate () : Gate ()
{
}

PermanentGate::PermanentGate( const AcGePoint3d& insertPt, double angle ) : Gate( insertPt, angle )
{

}

TemporaryGate::TemporaryGate () : Gate ()
{
}

TemporaryGate::TemporaryGate( const AcGePoint3d& insertPt, double angle ) : Gate( insertPt, angle )
{

}

DoubleGate::DoubleGate () : Gate ()
{
}

DoubleGate::DoubleGate( const AcGePoint3d& insertPt, double angle ) : Gate( insertPt, angle )
{

}

BalanceGate::BalanceGate () : Gate ()
{
}

BalanceGate::BalanceGate( const AcGePoint3d& insertPt, double angle ) : Gate( insertPt, angle )
{

}