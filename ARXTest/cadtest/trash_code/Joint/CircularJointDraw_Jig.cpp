#include "StdAfx.h"
#include "CircularJointDraw_Jig.h"

CircularJointDraw_Jig::CircularJointDraw_Jig( const CString& geType, const CString& drawName )
    : MineGEDraw_Jig( geType, drawName )
{
}

Adesk::Boolean CircularJointDraw_Jig::doJig( MineGEDraw* pMineGEDraw )
{
    // 转换成特定效果的draw指针对象
    m_pDraw = CircularJointDraw::cast( pMineGEDraw );

    setUserInputControls( ( UserInputControls )( kAcceptMouseUpAsPoint | kDontUpdateLastPoint ) );

    setDispPrompt( _T( "\n请选择节点的插入点坐标: " ) );
    AcGePoint3d pt;
    DragStatus stat = acquirePoint( pt );

    if ( stat != kNormal ) return Adesk::kFalse;

    m_pDraw->m_insertPt = pt;

    setDispPrompt( _T( "\n请输入节点的圆半径: " ) );
    stat = drag();

    return ( stat == kNormal );
}

AcEdJig::DragStatus CircularJointDraw_Jig::getRadius()
{
    //setUserInputControls((UserInputControls) kDontUpdateLastPoint);

    //acutPrintf(_T("\ncall getRadius()..."));
    double radius;
    AcEdJig::DragStatus stat = acquireDist( radius, m_pDraw->m_insertPt );

    if( stat != kNormal ) return stat;

    //acutPrintf(_T("\nget radius:%.3f\tcurrent radius:%.3f"),radius, m_pDraw->m_radius);

    if( abs( m_pDraw->m_radius - radius ) < 0.001 ) // 小于0.001，则近似可以看做相等
    {
        stat = kNoChange;
    }
    else
    {
        m_pDraw->m_radius = radius;
    }

    return stat;
}

AcEdJig::DragStatus CircularJointDraw_Jig::sampler()
{
    //acutPrintf(_T("\call sampler()  \tlast radius:%.3f"), m_pDraw->m_radius);
    return getRadius();
}

Adesk::Boolean CircularJointDraw_Jig::update()
{
    //acutPrintf(_T("\ncall update()..."));
    return Adesk::kTrue;
}

AcDbEntity* CircularJointDraw_Jig::entity() const
{
    //acutPrintf(_T("\ncall entity() \tradius:%.3f"), m_pDraw->m_radius);
    return m_pDraw;
}
