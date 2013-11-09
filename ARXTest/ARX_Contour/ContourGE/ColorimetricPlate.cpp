#include "StdAfx.h"
#include "ColorimetricPlate.h"

Adesk::UInt32 ColorimetricPlate::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    ColorimetricPlate, ContourGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    比色板, CONTOURAPP )

ColorimetricPlate::ColorimetricPlate() : ContourGE ()
{
}

ColorimetricPlate::~ColorimetricPlate ()
{
}

Acad::ErrorStatus ColorimetricPlate::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = ContourGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( ColorimetricPlate::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    // 写入比色板颜色
    pFiler->writeInt32( m_colors.length() );
    for( int i = 0; i < m_colors.length(); i++ )
    {
        pFiler->writeInt32( m_colors[i].r );
        pFiler->writeInt32( m_colors[i].g );
        pFiler->writeInt32( m_colors[i].b );
    }

    // 写入z值
    pFiler->writeInt32( m_zValues.length() );
    for( int i = 0; i < m_zValues.length(); i++ )
    {
        pFiler->writeDouble( m_zValues[i] );
    }

    // 写入几何信息
    pFiler->writePoint3d( m_pt );
    pFiler->writeDouble( m_width );
    pFiler->writeDouble( m_height );

    // 写入标注信息
    pFiler->writeDouble( m_distance );
    pFiler->writeDouble( m_textHeight );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus ColorimetricPlate::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = ContourGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > ColorimetricPlate::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    // 读取比色板颜色
    long n = 0;
    pFiler->readInt32( &n );
    m_colors.removeAll();
    for( int i = 0; i < ( int )n; i++ )
    {
        long r, g, b;
        pFiler->readInt32( &r );
        pFiler->readInt32( &g );
        pFiler->readInt32( &b );
        ARX_Color color = {( int )r, ( int )g, ( int )b};
        m_colors.append( color );
    }

    // 读取z值
    pFiler->readInt32( &n );
    m_zValues.removeAll();
    for( int i = 0; i < ( int )n; i++ )
    {
        double z;
        pFiler->readDouble( &z );
        m_zValues.append( z );
    }

    // 读取比色板几何信息
    pFiler->readPoint3d( &m_pt );
    pFiler->readDouble( &m_width );
    pFiler->readDouble( &m_height );

    // 读取标注信息
    pFiler->readDouble( &m_distance );
    pFiler->readDouble( &m_textHeight );

    return ( pFiler->filerStatus() ) ;
}

// 绘制一个颜色框
static void DrawColorRectangle(
    AcGiWorldDraw* mode,
    const AcGePoint3d& pt,
    double width, double height,
    int r, int g, int b )
{
    AcGiSubEntityTraits& traits = mode->subEntityTraits();

    // 备份颜色和填充类型
    AcGiFillType ft = traits.fillType();
    Adesk::UInt16 cl = traits.color();

    // 设置为填充封闭图形
    traits.setFillType( kAcGiFillAlways );
    AcCmEntityColor color;
    color.setRGB( r, g, b );
    // 设置颜色
    traits.setTrueColor( color );

    // 计算矩形框
    AcGePoint3dArray pts;
    pts.append( pt );

    AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    pts.append( pt + v1 * width );

    v2.rotateBy( -1 * PI / 2, AcGeVector3d::kZAxis );
    pts.append( pt + v1 * width + v2 * height );

    pts.append( pt + v2 * height );

    mode->geometry().polygon( pts.length(), pts.asArrayPtr() );

    traits.setFillType( ft );
    traits.setColor( cl );
}

static void DrawZValue( AcGiWorldDraw* mode, double z, const AcGePoint3d& tpt, double angle, double height )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );
    if( v.x < 0 )
    {
        angle += PI;
    }

    CString text;
    text.Format( _T( " %.5f " ), z );

    AcDbMText mt;
    mt.setLocation( tpt );
    mt.setTextHeight( height );
    mt.setAttachment( AcDbMText::kMiddleLeft );
    mt.setRotation( angle );
    mt.setBackgroundFill( true );
    mt.setContents( text );

    // 经过测试发现，AcDbMText调用worldDraw的同时会修改mode的一些属性
    mt.setColorIndex( mode->subEntityTraits().color() );

    mt.worldDraw( mode );
}

Adesk::Boolean ColorimetricPlate::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    if( !m_colors.isEmpty() )
    {
        int n = m_colors.length();
        double dw = m_height / n;
        for( int i = 0; i < n; i++ )
        {
            AcGeVector3d v( -AcGeVector3d::kYAxis );
            DrawColorRectangle( mode, m_pt + v * i * dw, m_width, dw, m_colors[i].r, m_colors[i].g, m_colors[i].b );
        }
    }

    return Adesk::kTrue;
}

Acad::ErrorStatus ColorimetricPlate::subTransformBy( const AcGeMatrix3d& xform )
{
    m_pt.transformBy( xform );

    AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( -AcGeVector3d::kYAxis );
    v1 *= m_width;
    v2 *= m_height;

    v1.transformBy( xform );
    v2.transformBy( xform );

    m_width = v1.length();
    m_height = v2.length();

    return Acad::eOk;
}

Acad::ErrorStatus ColorimetricPlate::subGetOsnapPoints (
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
        snapPoints.append( m_pt );
    }
    return Acad::eOk;
}

Acad::ErrorStatus ColorimetricPlate::subGetGripPoints ( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_pt );

    AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( -AcGeVector3d::kYAxis );
    gripPoints.append( m_pt + v1 * m_width + v2 * m_height );

    return Acad::eOk;
}

Acad::ErrorStatus ColorimetricPlate::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        // 始节点
        if ( idx == 0 )
        {
            m_pt += offset;
        }
        if( idx == 1 )
        {
            AcGeVector3d v1( AcGeVector3d::kXAxis );
            AcGeVector3d vv1 = v1 * m_width + v1.dotProduct( offset ) * v1; // 计算偏移offset在angle方向上的投影
            m_width = vv1.length();

            AcGeVector3d v2( -AcGeVector3d::kYAxis );
            AcGeVector3d vv2 = v2 * m_height + v2.dotProduct( offset ) * v2; // 计算偏移offset在angle方向上的投影
            m_height = vv2.length();
        }
    }
    return Acad::eOk;
}

void ColorimetricPlate::setSize( const AcGePoint3d& pt, double width, double height )
{
    assertWriteEnabled();
    m_pt = pt;
    m_width = width;
    m_height = height;
}


void ColorimetricPlate::setLabel( double distance, double textHeight )
{
    assertWriteEnabled();
    m_distance = distance;
    m_textHeight = textHeight;
}

void ColorimetricPlate::addColor( int r, int g, int b )
{
    assertWriteEnabled();

    ARX_Color color = {r, g, b};
    m_colors.append( color );
}

void ColorimetricPlate::addZValue( double z )
{
    assertWriteEnabled();
    m_zValues.append( z );
}

void ColorimetricPlate::clearAll()
{
    assertWriteEnabled();
    m_colors.removeAll();
    m_zValues.removeAll();
}

void ColorimetricPlate::getAllColors( ArxColorArray& colors )
{
    assertReadEnabled () ;
    colors.append( m_colors );
}

void ColorimetricPlate::getAllZValues( AcGeDoubleArray& zValues )
{
    assertReadEnabled () ;
    zValues.append( m_zValues );
}