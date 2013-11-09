#include "StdAfx.h"
#include "ARX_DrawContour.h"

#include "../ContourGE/ContourLine.h"
#include "../ContourGE/ContourLabel.h"
#include "../ContourGE/ContourFill.h"
#include "../ContourGE/ContourInfo.h"
#include "../ContourGE/ColorimetricPlate.h"

#include "../ArxHelper/HelperClass.h"

bool CreateContourLine( double z, const AcGePoint3dArray& pts, bool bSmooth )
{
    ContourLine* pContourLine = new ContourLine();
    pContourLine->setPoints( pts );
    pContourLine->setZValues( z );
    pContourLine->smooth( bSmooth );

    // 添加到数据库
    bool ret = ArxUtilHelper::PostToModelSpace( pContourLine );
    if( ret )
    {
        pContourLine->close();
    }
    else
    {
        delete pContourLine;
    }
    return ret;
}

bool CreateContourLabel( double z,
                         const AcGePoint3dArray& cnpts,
                         bool bSmooth,
                         const AcGePoint3d& tpt,
                         double textHeight )
{
    ContourLabel* pContourLabel = new ContourLabel();
    pContourLabel->setZValues( z );
    pContourLabel->setPoints( cnpts );
    pContourLabel->setPosition( tpt );
    pContourLabel->smooth( bSmooth );
    pContourLabel->setTextHeight( textHeight );

    pContourLabel->update();  // 计算标注位置

    // 添加到数据库
    bool ret = ArxUtilHelper::PostToModelSpace( pContourLabel );
    if( ret )
    {
        pContourLabel->close();
    }
    else
    {
        delete pContourLabel;
    }
    return ret;
}

void CreateContourLabels( double z,
                          const AcGePoint3dArray& cnpts,
                          bool bSmooth,
                          const AcGePoint3dArray& tpts,
                          double textHeight )
{
    //acutPrintf(_T("\nz=%.4f, 标注个数:%d"), z, tpts.length());
    int n = tpts.length();
    for( int i = 0; i < n; i++ )
    {
        //acutPrintf(_T("\n\t(%.3f, %.3f)"), tpts[i].x, tpts[i].y);
        CreateContourLabel( z, cnpts, bSmooth, tpts[i], textHeight );
    }
    //acutPrintf(_T("\n"));
}

bool CreateContourFill( const AcGePoint3dArray& pts, int cnpts_num, int bpts_num, COLORREF rgb, bool bSmooth )
{
    ContourFill* pFill = new ContourFill();
    pFill->setPolygon( pts, cnpts_num, bpts_num, rgb );
    pFill->smooth( bSmooth );

    // 添加到数据库
    bool ret = ArxUtilHelper::PostToModelSpace( pFill );
    if( ret )
    {
        pFill->close();
    }
    else
    {
        delete pFill;
    }
    return ret;
}

bool CreateColorimetricPlate( const AcGePoint3d& pt, double width, double height, const AcGeColorArray& all_colors )
{
    //ColorArray all_colors;
    //BuildColorArray(all_colors);

    ColorimetricPlate* pPlate = new ColorimetricPlate();
    pPlate->setSize( pt, width, height );
    for( int i = 0; i < all_colors.length(); i++ )
    {
        COLORREF clr = all_colors[i];
        pPlate->addColor( GetRValue( clr ), GetGValue( clr ), GetBValue( clr ) );
    }

    bool ret = ArxUtilHelper::PostToModelSpace( pPlate );
    if( !ret )
    {
        delete pPlate;
    }
    else
    {
        pPlate->close();
    }
    return ret;
}

bool CreateContourInfo( const AcGePoint3dArray& pts, const AcGeDoubleArray& zValues )
{
    ContourInfo* pColor = new ContourInfo();
    pColor->setDatas( pts );
    pColor->setZValues( zValues );
    pColor->needFill( false );

    // 添加到数据库
    bool ret = ArxUtilHelper::PostToModelSpace( pColor );
    if( ret )
    {
        pColor->close();
    }
    else
    {
        delete pColor;
    }
    return ret;
}