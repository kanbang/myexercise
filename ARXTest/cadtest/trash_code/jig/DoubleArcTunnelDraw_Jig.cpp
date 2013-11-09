#include "StdAfx.h"
#include "DoubleArcTunnelDraw_Jig.h"

DoubleArcTunnelDraw_Jig::DoubleArcTunnelDraw_Jig( const CString& geType, const CString& drawName )
    : MineGEDraw_Jig( geType, drawName )
{
}

Adesk::Boolean DoubleArcTunnelDraw_Jig::doJig( MineGEDraw* pMineGEDraw )
{
    // ת�����ض�Ч����drawָ�����
    m_pDraw = DoubleArcTunnelDraw::cast( pMineGEDraw );
    //m_pDraw->m_width = 60;

    setUserInputControls( ( UserInputControls )( kAcceptMouseUpAsPoint | kDontUpdateLastPoint ) );

    setDispPrompt( _T( "\n��ѡ������������: " ) );
    AcGePoint3d pt;
    DragStatus stat = acquirePoint( pt );
    if ( stat != kNormal ) return Adesk::kFalse;

    m_pDraw->m_startPt = pt;
    m_pDraw->m_endPt = pt;

    gs = SEPOINT;
    setDispPrompt( _T( "\n��ѡ�������2������: " ) );
    stat = drag();
    if ( ( stat != kNoChange ) && ( stat != kNormal ) ) return Adesk::kFalse;

    gs = EPOINT;
    setDispPrompt( _T( "\n��ѡ�����ĩ������: " ) );
    stat = drag();

    return ( stat == kNormal );
}

AcEdJig::DragStatus DoubleArcTunnelDraw_Jig::sampler()
{
    if( gs == SEPOINT )
        return getSecondPoint();
    else if( gs == EPOINT )
        return getEndPoint();
    else
        return kNormal;
}

Adesk::Boolean DoubleArcTunnelDraw_Jig::update()
{
    m_pDraw->m_thirdPt = m_secondPt;
    m_pDraw->update();

    return Adesk::kTrue;
}

AcDbEntity* DoubleArcTunnelDraw_Jig::entity() const
{
    return m_pDraw;
}

AcEdJig::DragStatus DoubleArcTunnelDraw_Jig::getEndPoint()
{
    AcGePoint3d pt;
    AcEdJig::DragStatus stat = acquirePoint( pt, m_secondPt );
    if( stat != kNormal ) return stat;

    if( pt == m_secondPt )
    {
        stat = kNoChange;
    }
    else
    {
        m_pDraw->m_endPt = pt;
    }
    return stat;
}

AcEdJig::DragStatus DoubleArcTunnelDraw_Jig::getSecondPoint()
{
    AcGePoint3d pt;
    AcEdJig::DragStatus stat = acquirePoint( pt, m_pDraw->m_startPt );
    if( stat != kNormal ) return stat;

    if( pt == m_pDraw->m_startPt || pt == m_secondPt )
    {
        stat = kNoChange;
    }
    else
    {
        m_secondPt = pt;
    }

    // �ڻ�õ�2���ʱ�򲻽��л���
    return kNoChange;
}