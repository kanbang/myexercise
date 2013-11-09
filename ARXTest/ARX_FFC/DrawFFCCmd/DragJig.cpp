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

    AcGeVector3d offset = dirPt - spt;          // �� dirPt - ept
    double L = offset.dotProduct( v );          // ��������offset������v�ϵ�ͶӰ
    if( L == 0 ) return AcGeVector3d::kIdentity; // ����������

    if( L < 0 )
    {
        v.negate();
    }
    return v;
}

bool DragJig::doIt()
{
    setDispPrompt( _T( "\n�ڹ���������ָ��һ����Ϊ�ƽ�����" ) );
    AcGePoint3d dirPt;
    if( kNormal != acquirePoint( dirPt ) ) return false;

    // ���㷽������
    m_dir = CaclDir( m_spt, m_ept, dirPt );
    if( m_dir.isZeroLength() ) return false;

    setDispPrompt( _T( "\nָ�����㣺" ) );
    if( kNormal != acquirePoint( m_basePt ) ) return false;

    setDispPrompt( _T( "\n�����빤�����ƽ����룺" ) );
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
     * !!!����Ҫ���ӽ��Ƶı仯����(NoChange)!!!
     * ���������ֱ�ӷ���,����
     *	DragStatus stat = acquireDist(m_dist, m_basePt);
     *  return stat;
     * ��ô������jigˢ�¹��죬����worldDraw�޷���������
     * ͼ�ξͲ���������ʾ
     */

    static double tempDist;
    DragStatus stat = acquireDist( m_dist, m_basePt );
    if ( abs( tempDist - m_dist ) > EPLISON ) // ���Ʋ����
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

    // ���¹���������
    m_pWS->setSEPoint( newSpt, newEpt );
    m_pWS->updateDraw();

    // ���¹���ʼ������
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

    // ���Ĺ���ĩ������
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

    // ���Ĺ����Ĳɿ���
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
