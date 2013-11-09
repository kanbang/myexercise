#include "stdafx.h"

#include "TunnelDragJig.h"
#include "DumyJigEntity.h"

static AcDbEntity* CloneEntity( const AcDbObjectId& objId )
{
    AcDbEntity* pEnt;
    acdbOpenObject( pEnt, objId, AcDb::kForRead );

    AcDbEntity* pClone = AcDbEntity::cast( pEnt->clone() );

    pEnt->close();

    return pClone;
}

static void CopyBack( AcDbEntity* pSrcEnt, const AcDbObjectId& objId )
{
    AcDbEntity* pEnt;
    acdbOpenAcDbEntity( pEnt, objId, AcDb::kForWrite );
    pEnt->copyFrom( pSrcEnt );
    pEnt->close();
}

TunnelDragJig::TunnelDragJig( const AcDbObjectId objId ) : m_objId( objId )
{
    m_pWS = Tunnel::cast( CloneEntity( objId ) );
    m_pWS->getSEPoint( spt, ept );
}

TunnelDragJig::~TunnelDragJig()
{
    delete m_pWS;
}

void TunnelDragJig::doIt()
{
    setDispPrompt( _T( "\n指定基点：" ) );
    if( kNormal != acquirePoint( basePt ) ) return;

    if( kNormal == drag() )
    {
        CopyBack( m_pWS, m_objId );
    }
}

AcEdJig::DragStatus TunnelDragJig::sampler()
{

    setUserInputControls( ( UserInputControls )
                          ( AcEdJig::kAccept3dCoordinates
                            | AcEdJig::kNoNegativeResponseAccepted
                            | AcEdJig::kNoZeroResponseAccepted ) );

    static AcGePoint3d tempPoint;

    DragStatus stat = acquirePoint( curPt, basePt );
    if( tempPoint != curPt )
    {
        tempPoint = curPt;
    }
    else if( stat == AcEdJig::kNormal )
    {
        return AcEdJig::kNoChange;
    }
    return stat;
}

Adesk::Boolean TunnelDragJig::update()
{
    AcGePlane plane;
    AcGeVector3d v = ept - spt;
    v.normalize();

    v.rotateBy( 0.5 * PI, AcGeVector3d::kZAxis );

    AcGeVector3d offset = curPt - basePt;
    double L = offset.dotProduct( v );
    if( L == 0 ) return false;

    if( L < 0 )
    {
        v.negate();
        L = -1 * L;
    }

    AcGePoint3d newSpt = spt + v * L, newEpt = ept + v * L;

    // 更新工作面坐标
    m_pWS->setSEPoint( newSpt, newEpt );
    m_pWS->updateDraw();

    return Adesk::kTrue;
}

AcDbEntity* TunnelDragJig::entity() const
{
    return m_pWS;
}
