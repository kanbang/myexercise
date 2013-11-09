#include "stdafx.h"

#include "GoafDragJig.h"
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

GoafDragJig::GoafDragJig( const AcDbObjectId objId, int pos ) : m_objId( objId ), m_pos( pos )
{
    m_pGoaf = Goaf::cast( CloneEntity( objId ) );
    m_pGoaf->getPolygon( m_polygon );
}

GoafDragJig::~GoafDragJig()
{
    delete m_pGoaf;
}

void GoafDragJig::doIt()
{
    setDispPrompt( _T( "\n指定基点：" ) );
    if( kNormal != acquirePoint( basePt ) ) return;

    if( kNormal == drag() )
    {
        CopyBack( m_pGoaf, m_objId );
    }
}

AcEdJig::DragStatus GoafDragJig::sampler()
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

Adesk::Boolean GoafDragJig::update()
{
    AcGeVector3d offset = curPt - basePt;

    int n = m_polygon.length();
    int p1 = m_pos, p2 = ( m_pos + 1 ) % n;
    m_pGoaf->setPoint( p1, m_polygon[p1] + offset );
    m_pGoaf->setPoint( p2, m_polygon[p2] + offset );
    m_pGoaf->updateDraw();

    return Adesk::kTrue;
}

AcDbEntity* GoafDragJig::entity() const
{
    return m_pGoaf;
}
