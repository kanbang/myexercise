#include "StdAfx.h"
#include "DoubleTTunnelDraw_Jig.h"

DoubleTTunnelDraw_Jig::DoubleTTunnelDraw_Jig( const CString& geType, const CString& drawName )
    : MineGEDraw_Jig( geType, drawName )
{
}

Adesk::Boolean DoubleTTunnelDraw_Jig::doJig( MineGEDraw* pMineGEDraw )
{
    // 转换成特定效果的draw指针对象
    m_pDraw = DoubleTTunnelDraw::cast( pMineGEDraw );

    // 简单起见，就不执行jig过程了
    m_pDraw->m_distance = 50;
    m_pDraw->m_width = 30;

    setUserInputControls( ( UserInputControls )( kAcceptMouseUpAsPoint | kDontUpdateLastPoint ) );

    setDispPrompt( _T( "\n请选择巷道起点坐标: " ) );
    AcGePoint3d pt;
    DragStatus stat = acquirePoint( pt );
    if ( stat != kNormal ) return Adesk::kFalse;

    m_pDraw->m_startPt = pt;

    gs = EPOINT;
    setDispPrompt( _T( "\n请选择巷道末点坐标: " ) );
    stat = drag();
    //if (stat != kNormal) return Adesk::kFalse;

    //gs = WIDTH;
    //setDispPrompt(_T("\n指定巷道宽度: "));
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

    if( abs( m_pDraw->m_width - width ) < 0.001 ) // 小于0.001，则近似可以看做相等
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
    m_pDraw->update(); // 更新参数

    // 可以根据gs来判断是否需要更新
    // 如果首先动态获取巷道宽度，这时可以不需要显示图形
    // 应返回Adesk::kFalse
    return Adesk::kTrue;
}

AcDbEntity* DoubleTTunnelDraw_Jig::entity() const
{
    return m_pDraw;
}