#include "StdAfx.h"

#include "VNG_EdgeSimpleDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS( VNG_EdgeSimpleDraw, MineGEDraw, 1 )

VNG_EdgeSimpleDraw::VNG_EdgeSimpleDraw () : MineGEDraw ()
{
}

VNG_EdgeSimpleDraw::~VNG_EdgeSimpleDraw ()
{
}

void VNG_EdgeSimpleDraw::setAllExtraParamsToDefault()
{
}

void VNG_EdgeSimpleDraw::configExtraParams()
{

}

void VNG_EdgeSimpleDraw::updateExtraParams()
{

}

void VNG_EdgeSimpleDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_spt );
    writer.writePoint( m_spt );
    writer.writePoint( m_pt );
    writer.writeDouble( m_arrowWidth );
    writer.writeDouble( m_arrowLength );
    writer.writeBoolean( m_needEdge );
    writer.writePoint( m_tpt );
    writer.writeDouble( m_textHeight );
    writer.writeInt( m_id );
}

void VNG_EdgeSimpleDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_spt );
    reader.readPoint( m_spt );
    reader.readPoint( m_pt );
    reader.readDouble( m_arrowWidth );
    reader.readDouble( m_arrowLength );
    reader.readBoolean( m_needEdge );
    reader.readPoint( m_tpt );
    reader.readDouble( m_textHeight );
    reader.readInt( m_id );
}

void VNG_EdgeSimpleDraw::readExtraParam( DrawParamReader& reader )
{
}

void VNG_EdgeSimpleDraw::writeExtraParam( DrawParamWriter& writer )
{
}

static double CacArcAngle( const AcGePoint3d& spt, const AcGePoint3d& ept, const AcGePoint3d& pt, double L )
{
    AcGeCircArc3d arc( spt, pt, ept );
    int c = ( arc.normal().z > 0 ? 1 : -1 ); // 弧线方向

    double dc = 0.95 * 0.5;
    double da = acos( dc * L / arc.radius() );
    AcGeVector3d v = pt - arc.center();
    v.rotateBy( c * ( PI - da ), AcGeVector3d::kZAxis );

    return v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
}

Adesk::Boolean VNG_EdgeSimpleDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    AcGiSubEntityTraits& traits = mode->subEntityTraits();
    Adesk::UInt16 cl = traits.color();
    if( m_id == -1 )
    {
        traits.setColor( 2 ); // 虚分支用黄色高亮
    }

    // 绘制分支(样条曲线)
    DrawArc( mode, m_spt, m_pt, m_ept, false );

    // 绘制箭头
    DrawArrow( mode, m_pt, CacArcAngle( m_spt, m_ept, m_pt, m_arrowLength ), m_arrowWidth, m_arrowLength );

    if( m_needEdge )
    {
        CString idStr;
        idStr.Format( _T( "%d" ), m_id );
        CString str;
        str.Format( _T( "e%s" ), MakeLowerText( idStr ) );
        DrawMText( mode, m_tpt, 0, str, m_textHeight );
    }

    // 复原颜色
    traits.setColor( cl );

    return Adesk::kTrue;
    //return (VNG_GE::subWorldDraw (mode)) ;
}

Acad::ErrorStatus VNG_EdgeSimpleDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    // 构造弧线，进行变换
    m_spt.transformBy( xform );
    m_ept.transformBy( xform );
    m_pt.transformBy( xform );

    if( m_needEdge )
    {
        m_tpt.transformBy( xform );
    }

    return Acad::eOk;
}

Acad::ErrorStatus VNG_EdgeSimpleDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // 弧线的捕捉点
    if( osnapMode == AcDb::kOsModeEnd )
    {
        snapPoints.append( m_spt );
        snapPoints.append( m_ept );
        snapPoints.append( m_pt );
        if( m_needEdge )
        {
            snapPoints.append( m_tpt );
        }
    }

    return Acad::eOk;
}

Acad::ErrorStatus VNG_EdgeSimpleDraw::subGetGripPoints(
    AcGePoint3dArray& gripPoints,
    AcDbIntArray& osnapModes,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_spt );
    gripPoints.append( m_ept );
    gripPoints.append( m_pt );

    if( m_needEdge )
    {
        gripPoints.append( m_tpt );
    }

    return Acad::eOk;
}

Acad::ErrorStatus VNG_EdgeSimpleDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    int n = indices.length();
    for( int i = 0; i < n; i++ )
    {
        int idx = indices[i];
        if( idx == 0 )
        {
            m_spt += offset;
        }
        else if( idx == 1 )
        {
            m_ept += offset;
        }
        else if( idx == 2 )
        {
            m_pt += offset;
        }
        else if( m_needEdge && idx == 3 )
        {
            m_tpt += offset;
        }
    }

    return Acad::eOk;
}