#include "StdAfx.h"

#include "VNG_NodeSimpleDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS( VNG_NodeSimpleDraw, MineGEDraw, 1 )

VNG_NodeSimpleDraw::VNG_NodeSimpleDraw () : MineGEDraw ()
{
}

VNG_NodeSimpleDraw::~VNG_NodeSimpleDraw ()
{
}


void VNG_NodeSimpleDraw::setAllExtraParamsToDefault()
{
}

void VNG_NodeSimpleDraw::configExtraParams()
{

}

void VNG_NodeSimpleDraw::updateExtraParams()
{

}

void VNG_NodeSimpleDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_pt );
    writer.writeDouble( m_width );
    writer.writeDouble( m_height );
    writer.writeDouble( m_textHeight );
    writer.writeInt( m_id );
}

void VNG_NodeSimpleDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_pt );
    reader.readDouble( m_width );
    reader.readDouble( m_height );
    reader.readDouble( m_textHeight );
    reader.readInt( m_id );
}

void VNG_NodeSimpleDraw::readExtraParam( DrawParamReader& reader )
{
}

void VNG_NodeSimpleDraw::writeExtraParam( DrawParamWriter& writer )
{
}

void VNG_NodeSimpleDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    // 计算椭圆的矩形框
    AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kYAxis );
    v1 *= m_width * 0.5;
    v2 *= m_height * 0.5;

    pts.append( m_pt + v1 + v2 );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( m_pt + v1 + v2 );

    v2.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( m_pt + v1 + v2 );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( m_pt + v1 + v2 );
}

Adesk::Boolean VNG_NodeSimpleDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    DrawEllipse( mode, m_pt, m_width, m_height, false );
    //DrawNode(mode, m_pt, m_width, m_height);

    CString idStr;
    idStr.Format( _T( "%d" ), m_id );

    CString str;
    str.Format( _T( "V%s" ), MakeLowerText( idStr ) );
    DrawMText( mode, m_pt, 0, str, m_textHeight );

    return Adesk::kTrue;
    //return (VNG_GE::subWorldDraw (mode));
}

Acad::ErrorStatus VNG_NodeSimpleDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    // 构造一个椭圆，进行变换
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v *= m_width / 2;
    AcDbEllipse ellipse( m_pt, AcGeVector3d::kZAxis, v, m_height / m_width );
    ellipse.transformBy( xform );

    // 构造一个字体对象进行变换
    AcDbMText mt;
    mt.setContents( _T( "xx" ) );
    mt.setTextHeight( m_textHeight );
    mt.setRotation( 0 );
    mt.setLocation( m_pt );
    mt.transformBy( xform );

    // 更新参数
    m_pt = ellipse.center();
    m_width = ellipse.majorAxis().length() * 2;
    m_height = ellipse.minorAxis().length() * 2;

    // 更新参数
    m_textHeight = mt.textHeight();

    return Acad::eOk;
}

Acad::ErrorStatus VNG_NodeSimpleDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    if( osnapMode == AcDb::kOsModeCen )
    {
        snapPoints.append( m_pt );
    }
    return Acad::eOk;
}

Acad::ErrorStatus VNG_NodeSimpleDraw::subGetGripPoints(
    AcGePoint3dArray& gripPoints,
    AcDbIntArray& osnapModes,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_pt );

    return Acad::eOk;
}

Acad::ErrorStatus VNG_NodeSimpleDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );

        if ( idx == 0 )
        {
            m_pt += offset;
        }
    }
    return Acad::eOk;
}
