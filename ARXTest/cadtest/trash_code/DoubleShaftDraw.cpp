#include "StdAfx.h"
#include "DoubleShaftDraw.h"

ACRX_CONS_DEFINE_MEMBERS ( DoubleShaftDraw, DoubleTunnelDraw, 1 )

DoubleShaftDraw::DoubleShaftDraw()
{
}

void DoubleShaftDraw::setAllExtraParamsToDefault()
{
    DoubleTunnelDraw::setAllExtraParamsToDefault();
    m_lineWeight = AcDb::kLnWt050;
}

void DoubleShaftDraw::configExtraParams()
{

}

void DoubleShaftDraw::readExtraParam( DrawParamReader& reader )
{
    DoubleTunnelDraw::readExtraParam( reader );
    int lw;
    reader.readInt( lw );
    m_lineWeight = ( AcDb::LineWeight )lw;
}

void DoubleShaftDraw::writeExtraParam( DrawParamWriter& writer )
{
    DoubleTunnelDraw::writeExtraParam( writer );
    writer.writeInt( m_lineWeight );
}

Adesk::Boolean DoubleShaftDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    AcDb::LineWeight lw = mode->subEntityTraits().lineWeight();
    //Adesk::UInt16 color = mode->subEntityTraits().color();
    mode->subEntityTraits().setLineWeight( m_lineWeight );
    //mode->subEntityTraits().setColor(5); // 蓝色

    if( m_startPt == m_endPt ) // 始末点重合
    {
        // 绘制成圆环形
        mode->geometry().circle( m_startPt, m_width * 0.5, AcGeVector3d::kZAxis ); // 外环
        mode->geometry().circle( m_startPt, m_width * 0.3, AcGeVector3d::kZAxis ); // 内环(外环半径的60%)
    }
    else
    {
        // 调用父类的绘制方法
        DoubleTunnelDraw::subWorldDraw( mode );
    }

    // 恢复原有的设置
    mode->subEntityTraits().setLineWeight( lw );
    //mode->subEntityTraits().setColor(color);

    return Adesk::kTrue;
}

//- Grip points protocol
Acad::ErrorStatus DoubleShaftDraw::subGetGripPoints ( AcGePoint3dArray& gripPoints,
        AcDbIntArray& osnapModes,
        AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_startPt );

    if( m_startPt == m_endPt )
    {
        AcGePoint3d pt( m_startPt );
        pt.x = pt.x + m_width * 0.3; // 外环半径的60%
        gripPoints.append( pt );
    }
    else
    {
        gripPoints.append( m_endPt );
    }

    return Acad::eOk;
}

Acad::ErrorStatus DoubleShaftDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );

        if ( idx == 0 )
        {
            m_startPt += offset;
            updateExtraParams();
        }

        if ( idx == 1 )
        {
            if( m_startPt == m_endPt ) // 始末点重合
            {
                AcGePoint3d pt( m_startPt );
                pt.x = pt.x + m_width * 0.3; // 外环半径的60%
                pt += offset;
                m_endPt = pt;
            }
            else
            {
                m_endPt += offset;
            }
            updateExtraParams();
        }
    }
    return Acad::eOk;
}