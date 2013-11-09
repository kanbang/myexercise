#include "stdafx.h"

#include "DragJig.h"
#include "DumyJigEntity.h"

#define EPLISON 0.001

DragJig::DragJig(
    const AcDbObjectId ws_objId,
    JunctionEdgeInfoArray& sptLinesInfo,
    JunctionEdgeInfoArray& eptLinesInfo,
    const AcDbObjectId& goaf_objId,
    int p1, int p2 )
    : m_sptLinesInfo( sptLinesInfo ),
      m_eptLinesInfo( eptLinesInfo ),
      m_dist( 0 ), m_p1( p1 ), m_p2( p2 ),
      m_pWS( 0 ), m_pGoaf( 0 )
{
    /*DumyJigEntity* */m_pJigEnt = new DumyJigEntity();
    m_pJigEnt->addEntity( ws_objId );

    int m = sptLinesInfo.length(), n = eptLinesInfo.length();
    for( int i = 0; i < m; i++ )
    {
        m_pJigEnt->addEntity( sptLinesInfo[i].objId );
    }
    for( int i = 0; i < n; i++ )
    {
        m_pJigEnt->addEntity( eptLinesInfo[i].objId );
    }
    if( !goaf_objId.isNull() )
    {
        m_pJigEnt->addEntity( goaf_objId );
    }

    m_pWS = WorkSurface::cast( m_pJigEnt->getEnity( 0 ) );
    for( int i = 0; i < m; i++ )
    {
        m_pSptLines.append( Tunnel::cast( m_pJigEnt->getEnity( i + 1 ) ) );
    }
    for( int i = 0; i < n; i++ )
    {
        m_pEptLines.append( Tunnel::cast( m_pJigEnt->getEnity( i + m + 1 ) ) );
    }
    if( !goaf_objId.isNull() )
    {
        m_pGoaf = Goaf::cast( m_pJigEnt->getEnity( m + n + 1 ) );
        m_pGoaf->getPolygon( m_polygon );
    }

    m_pWS->getSEPoint( m_spt, m_ept );
}

DragJig::~DragJig()
{
    delete m_pJigEnt;
}

static AcGeVector3d CaclDir( const AcGePoint3d& spt, const AcGePoint3d& ept, const AcGePoint3d& dirPt )
{
    if( dirPt == spt || dirPt == ept ) return AcGeVector3d::kIdentity;
    AcGeVector3d v = ept - spt;
    v.normalize();
    v.rotateBy( 0.5 * PI, AcGeVector3d::kZAxis );

    AcGeVector3d offset = dirPt - spt;          // 或 dirPt - ept
    double L = offset.dotProduct( v );          // 计算向量offset在向量v上的投影
    if( L == 0 ) return AcGeVector3d::kIdentity; // 返回零向量

    if( L < 0 )
    {
        v.negate();
    }
    return v;
}

bool DragJig::doIt()
{
    setDispPrompt( _T( "\n在工作面两侧指定一点作为推进方向：" ) );
    AcGePoint3d dirPt;
    if( kNormal != acquirePoint( dirPt ) ) return false;

    // 计算方向向量
    m_dir = CaclDir( m_spt, m_ept, dirPt );
    if( m_dir.isZeroLength() ) return false;

    setDispPrompt( _T( "\n指定基点：" ) );
    if( kNormal != acquirePoint( m_basePt ) ) return false;

    setDispPrompt( _T( "\n请输入工作面推进距离：" ) );
    bool ret = ( kNormal == drag() );
    if( ret )
    {
        m_pJigEnt->writeBack();
    }

    return ret;
}

AcEdJig::DragStatus DragJig::sampler()
{
    setUserInputControls( ( UserInputControls )
                          ( AcEdJig::kAccept3dCoordinates
                            | AcEdJig::kNoNegativeResponseAccepted
                            | AcEdJig::kNoZeroResponseAccepted ) );

    //static AcGePoint3d tempPoint;
    //DragStatus stat = acquirePoint(curPt, m_basePt);
    //if(tempPoint != curPt)
    //{
    //	tempPoint = curPt;
    //	m_dist = curPt.distanceTo(m_basePt);
    //}
    //else if(stat == AcEdJig::kNormal)
    //{
    //	return AcEdJig::kNoChange;
    //}

    /*
     * !!!必须要增加近似的变化处理(NoChange)!!!
     * 如果不处理直接返回,例如
     *	DragStatus stat = acquireDist(m_dist, m_basePt);
     *  return stat;
     * 那么会由于jig刷新过快，导致worldDraw无法正常工作
     * 图形就不能正常显示
     */

    static double tempDist;
    DragStatus stat = acquireDist( m_dist, m_basePt );
    if ( abs( tempDist - m_dist ) > EPLISON ) // 近似不相等
    {
        tempDist = m_dist;
    }
    else if( stat == AcEdJig::kNormal )
    {
        return AcEdJig::kNoChange;
    }

    return stat;
}

Adesk::Boolean DragJig::update()
{
    //AcGeVector3d v = m_ept - m_spt;
    //v.normalize();

    //v.rotateBy(0.5*PI, AcGeVector3d::kZAxis);

    //AcGeVector3d offset = curPt - m_basePt;
    //double L = offset.dotProduct(v);
    //if(L == 0) return false;

    //if(L < 0)
    //{
    //	v.negate();
    //	L = -1*L;
    //}

    //AcGePoint3d newSpt = m_spt+v*L, newEpt = m_ept+v*L;
    AcGePoint3d newSpt = m_spt + m_dir * m_dist, newEpt = m_ept + m_dir * m_dist;

    // 更新工作面坐标
    m_pWS->setSEPoint( newSpt, newEpt );
    m_pWS->updateDraw();

    // 更新关联始点的巷道
    int m = m_sptLinesInfo.length();
    for( int i = 0; i < m; i++ )
    {
        Tunnel* pTunnel = ( Tunnel* )m_pSptLines[i];
        bool startOrEnd = m_sptLinesInfo[i].startOrEnd;

        AcGePoint3d p1, p2;
        pTunnel->getSEPoint( p1, p2 );

        if( startOrEnd )
        {
            pTunnel->setSEPoint( newSpt, p2 );
        }
        else
        {
            pTunnel->setSEPoint( p1, newSpt );
        }
        pTunnel->updateDraw();
    }

    // 更改关联末点的巷道
    int n = m_eptLinesInfo.length();
    for( int i = 0; i < n; i++ )
    {
        Tunnel* pTunnel = ( Tunnel* )m_pEptLines[i];
        bool startOrEnd = m_eptLinesInfo[i].startOrEnd;

        AcGePoint3d p1, p2;
        pTunnel->getSEPoint( p1, p2 );

        if( startOrEnd )
        {
            pTunnel->setSEPoint( newEpt, p2 );
        }
        else
        {
            pTunnel->setSEPoint( p1, newEpt );
        }
        pTunnel->updateDraw();
    }

    // 更改关联的采空区
    if( m_pGoaf != 0 )
    {
        m_pGoaf->setPoint( m_p1, newSpt );
        m_pGoaf->setPoint( m_p2, newEpt );
        m_pGoaf->updateDraw();
    }
    return Adesk::kTrue;
}

AcDbEntity* DragJig::entity() const
{
    return m_pJigEnt;
}
