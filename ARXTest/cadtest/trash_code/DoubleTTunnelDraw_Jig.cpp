#include "StdAfx.h"
#include "DoubleTTunnelDraw_Jig.h"

DoubleTTunnelDraw_Jig::DoubleTTunnelDraw_Jig( const CString& geType, const CString& drawName )
    : MineGEDraw_Jig( geType, drawName )
{
}

Adesk::Boolean DoubleTTunnelDraw_Jig::doJig( MineGEDraw* pMineGEDraw )
{
    // ת�����ض�Ч����drawָ�����
    m_pDraw = DoubleTTunnelDraw::cast( pMineGEDraw );

    // ��������Ͳ�ִ��jig������
    m_pDraw->m_distance = 50;
    m_pDraw->m_width = 30;

    setUserInputControls( ( UserInputControls )( kAcceptMouseUpAsPoint | kDontUpdateLastPoint ) );

    setDispPrompt( _T( "\n��ѡ������������: " ) );
    AcGePoint3d pt;
    DragStatus stat = acquirePoint( pt );
    if ( stat != kNormal ) return Adesk::kFalse;

    m_pDraw->m_startPt = pt;

    gs = EPOINT;
    setDispPrompt( _T( "\n��ѡ�����ĩ������: " ) );
    stat = drag();
    //if (stat != kNormal) return Adesk::kFalse;

    //gs = WIDTH;
    //setDispPrompt(_T("\nָ��������: "));
    //stat = drag();

    return ( stat == kNormal );
}

AcEdJig::DragStatus DoubleTTunnelDraw_Jig::getEndPoint()
{
    AcGePoint3d pt;
    AcEdJig::DragStatus stat = acquirePoint( pt, m_pDraw->m_startPt );
    if( stat != kNormal ) return stat;

    if( pt == m_pDraw->m_startPt )
    {
        stat = kNoChange;
    }
    else
    {
        m_pDraw->m_endPt = pt;
    }
    return stat;
}

AcEdJig::DragStatus DoubleTTunnelDraw_Jig::getWidth()
{
    double width;
    AcEdJig::DragStatus stat = acquireDist( width, m_pDraw->m_endPt );
    if( stat != kNormal ) return stat;

    if( abs( m_pDraw->m_width - width ) < 0.001 ) // С��0.001������ƿ��Կ������
    {
        stat = kNoChange;
    }
    else
    {
        m_pDraw->m_width = width;
    }
    return stat;
}

AcEdJig::DragStatus DoubleTTunnelDraw_Jig::sampler()
{
    if( gs == EPOINT )
    {
        //acutPrintf(_T("\ncall getEndPoint()..."));
        return getEndPoint();
    }
    else if( gs == WIDTH )
    {
        //acutPrintf(_T("\ncall getWidth()..."));
        return getWidth();
    }
    else
    {
        //acutPrintf(_T("\ncall nothing()..."));
        return kNormal;
    }
}

Adesk::Boolean DoubleTTunnelDraw_Jig::update()
{
    m_pDraw->update(); // ���²���

    // ���Ը���gs���ж��Ƿ���Ҫ����
    // ������ȶ�̬��ȡ�����ȣ���ʱ���Բ���Ҫ��ʾͼ��
    // Ӧ����Adesk::kFalse
    return Adesk::kTrue;
}

AcDbEntity* DoubleTTunnelDraw_Jig::entity() const
{
    return m_pDraw;
}