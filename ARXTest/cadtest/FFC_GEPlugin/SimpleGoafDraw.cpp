#include "StdAfx.h"
#include "SimpleGoafDraw.h"

#include "DrawTool.h"

/* 全局函数(HatchRatio, 计算填充比例) */
extern double CaclHatchRatio( const AcGePoint3dArray& polygon );

ACRX_CONS_DEFINE_MEMBERS ( SimpleGoafDraw, MineGEDraw, 1 )

SimpleGoafDraw::SimpleGoafDraw () : MineGEDraw ()
{
}

SimpleGoafDraw::~SimpleGoafDraw ()
{
}

void SimpleGoafDraw::setAllExtraParamsToDefault()
{
    m_scale = 1.0;
    m_width = 40;
}

void SimpleGoafDraw::configExtraParams()
{

}

void SimpleGoafDraw::updateExtraParams()
{
    // 根据多边形的边长计算缩放比例
    if( m_pts.length() < 3 ) return;
    m_scale = CaclHatchRatio( m_pts );
}

void SimpleGoafDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writeInt( m_pts.length() );
    for( int i = 0; i < m_pts.length(); i++ )
    {
        writer.writePoint( m_pts[i] );
    }
}

void SimpleGoafDraw::readKeyParam( DrawParamReader& reader )
{
    int n = 0;
    reader.readInt( n );

    m_pts.removeAll();
    for( int i = 0; i < n; i++ )
    {
        AcGePoint3d pt;
        reader.readPoint( pt );
        m_pts.append( pt );
    }
}

void SimpleGoafDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_scale );
    reader.readDouble( m_width );
}

void SimpleGoafDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_scale );
    writer.writeDouble( m_width );
}

static void DrawBoundary( AcGiWorldDraw* mode, const AcGePoint3dArray& polygon, double offset )
{
    AcGePoint3dArray offset_polygon;
    if( OffSetPolygon( polygon, offset, false, offset_polygon ) )
    {
        DrawPolygon( mode, offset_polygon, false );
    }
}

static void DrawGoaf( AcGiWorldDraw* mode, const AcGePoint3dArray& polygon, double offset, double scale )
{
    AcGePoint3dArray offset_polygon;
    if( OffSetPolygon( polygon, offset, true, offset_polygon ) )
    {
        DrawPolygonHatch( mode, offset_polygon, _T( "GRAVEL" ), scale );
    }
}

Adesk::Boolean SimpleGoafDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    if( m_pts.length() > 2 )
    {
        // 绘制外框
        DrawBoundary( mode, m_pts, m_width * 0.5 );

        // 绘制采空区填充
        //acutPrintf(_T("\nscale:%.5f"), m_scale);
        const double c = 1.0;
        DrawGoaf( mode, m_pts, m_width * 0.5, c * m_scale );
    }

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleGoafDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    for( int i = 0; i < m_pts.length(); i++ )
    {
        m_pts[i].transformBy( xform );
    }

    AcGeVector3d v( AcGeVector3d::kXAxis );
    v *= m_scale;
    v.transformBy( xform );
    m_scale = v.length();

    return Acad::eOk;
}

Acad::ErrorStatus SimpleGoafDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    if( osnapMode == AcDb::kOsModeEnd )
    {
        snapPoints.append( m_pts );
    }

    return Acad::eOk;
}

Acad::ErrorStatus SimpleGoafDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_pts );

    return Acad::eOk;
}

Acad::ErrorStatus SimpleGoafDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    int n = m_pts.length();

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        if( idx >= 0 && idx < n )
        {
            AcGePoint3d pt = m_pts[idx] + offset;
            if( !m_pts.contains( pt ) )
            {
                m_pts[idx] = pt;
            }
        }
    }

    updateExtraParams();

    return Acad::eOk;
}

void SimpleGoafDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    /*AcGePoint3dArray polygon;
    OffSetPolygon(m_pts, m_width*0.5, true, polygon);
    pts.append(polygon);*/
}