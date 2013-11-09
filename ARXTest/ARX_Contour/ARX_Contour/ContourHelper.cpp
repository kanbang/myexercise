#include "StdAfx.h"
#include "ContourHelper.h"

#include "ARX_GeoAlgo.h"
#include "ARX_ContourAlgo.h"
#include "ARX_DrawContour.h"

#include "CADDrawHelper.h"

#include "../ArxHelper/HelperClass.h"

// pts.length() == nDiameter*nDiameter
// 坐标点的z值保存使用插值算法计算的值
static void DrawGrid( int nDiameter, const AcGePoint3dArray& pts, double textHeight )
{
    // 绘制原始数据
    // 绘制网格
    for( int i = 0; i < nDiameter; i++ )
    {
        for( int j = 0; j < nDiameter; j++ )
        {
            AcGePoint3d pt = pts[i * nDiameter + j];

            CString text;
            text.Format( _T( "%.3f" ), pt.z );

            pt.z = 0;
            DrawMText( pt, 0, text, textHeight ); // 标注文字
        }

        AcGePoint3dArray tempPts;
        for( int j = 0; j < nDiameter; j++ )
        {
            AcGePoint3d pt = pts[i * nDiameter + j];
            pt.z = 0;

            tempPts.append( pt );
        }
        DrawLines( tempPts );

        tempPts.removeAll();
        for( int j = 0; j < nDiameter; j++ )
        {
            AcGePoint3d pt = pts[j * nDiameter + i];
            pt.z = 0;

            tempPts.append( pt );
        }
        DrawLines( tempPts );
    }
}

/*
 * 网格计算的思路
 * 指定不规则多边形，划分网格
 * 网格中可能存在不规则的形状，例如三角形，四边形等
 * 计算每一个多边形的平均z值(通常直接采用平均方法，采用插值方法计算多边形顶点的z值，然后求平均)
 * 计算不规则多边形中的所有"网格"的面积*平均z值，并求总和，然后除以整个多边形的面积
 * 得到多边形总的平均z值
 *
 * 下面的代码思路：
 * 1) 将所有的多边形都处理成矩形，显然每一个小矩形的面积都是相同的
 * 2) 将网格划分成(n+1)*(n+1)个，取每一个小矩形的中心点进行插值，作为矩形的平均值
 * 3) 由于矩形的面积是相同的，可以简化成所有中心点z值求和取平均值
 *
 * 计算很可能存在误差
 ------------------------------------------------
 |		-		|		-		|		-		|
 ------------------------------------------------
 |		-		|		-		|		-		|
 ------------------------------------------------
 |		-		|		-		|		-		|
 ------------------------------------------------
 |		-		|		-		|		-		|
 ------------------------------------------------
 如上，要计算3*3的网格，实际将网格划分成4*4
 */
double ContourHelper::GridCacl( const AcGePoint3dArray& datas, const AcGePoint3d& minPt, const AcGePoint3d& maxPt, int nDiameter )
{
    // 每一个小网格的宽(dx)和高(dy)
    double dx = ( maxPt.x - minPt.x ) / ( nDiameter + 1 );
    double dy = ( maxPt.y - minPt.y ) / ( nDiameter + 1 );

    // 字体高度
    double c = 0.2;
    double textHeight = int( std::min( dx, dy ) * c );

    AcGePoint3d basePt = minPt;
    basePt.x = basePt.x + dx / 2;
    basePt.y = basePt.y + dy / 2;

    // 准备网格坐标数据
    AcGePoint3dArray pts;
    for( int i = 0; i < nDiameter; i++ )
    {
        for( int j = 0; j < nDiameter; j++ )
        {
            // 坐标系从左下角开始
            double xpos = basePt.x + j * dx;
            double ypos = basePt.y + i * dy;
            pts.append( AcGePoint3d( xpos, ypos, 0 ) );
        }
    }

    // 使用Kriging插值算法，返回值保存在pts的z值
    ARX_InterpolatePoints( datas, pts );

    // 绘制原始数据
    //DrawOrginData(datas, 4);
    //DrawGrid(nDiameter, pts, textHeight);

    // 计算平均值
    double sum = 0;
    int len = pts.length();
    for( int i = 0; i < len; i++ )
    {
        sum += pts[i].z;
    }
    return sum / len;
}

double ContourHelper::PointCacl( const AcGePoint3dArray& datas, const AcGePoint3d& pt )
{
    return ARX_InterpolatePoint( datas, pt );
}

void ContourHelper::GetMinMaxZValue( const AcGePoint3dArray& datas, double& minZ, double& maxZ )
{
    ARX_GetMinMaxZValue( datas, minZ, maxZ );
}

void ContourHelper::DrawCountourByNum( AcGePoint3dArray& bounds, const AcGePoint3dArray& pts, double minZ, double maxZ, int n )
{
    DrawCountourByDeltaZ( bounds, pts, minZ, maxZ, ( maxZ - minZ ) / n );
}

void ContourHelper::DrawCountourByDeltaZ( AcGePoint3dArray& bounds, const AcGePoint3dArray& pts, double minZ, double maxZ, double dz )
{
    ARX_DrawCountour( bounds, pts, minZ, maxZ, dz );
}

void ContourHelper::DrawCountourFill( AcGePoint3dArray& bounds, const AcGePoint3dArray& pts, const AcGeDoubleArray& zValues, const AcGeColorArray& colors )
{
    ARX_DrawFill( bounds, pts, zValues, colors );
}

// 测试代码
static void BuildColorArray( AcGeColorArray& all_colors )
{
    const int rgb[][3] =
    {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
        {0, 255, 128},
        {0, 128, 192},
        {255, 255, 128},
        {128, 255, 128},
        {128, 255, 255},
        {0, 128, 255},
        {255, 128, 192},
        {255, 128, 255},
        {128, 128, 255},
        {0, 64, 128},
        {128, 128, 192},
        {255, 255, 0},
        {128, 255, 0},
        {0, 255, 64},
        {0, 128, 128},
        {0, 255, 255},
        {0, 255, 0},
        {255, 128, 64},
        {128, 0, 64},
        {255, 0, 255}
    };

    // 元素总数:
    int n = sizeof( rgb ) / ( sizeof( rgb[0][0] ) );
    // 数组行数:
    int row = sizeof( rgb ) / ( sizeof( rgb[0] ) );
    // 列数:
    int col = n / row;

    for( int i = 0; i < row; i++ )
    {
        all_colors.append( RGB( rgb[i][0], rgb[i][1], rgb[i][2] ) );
    }
}

void ContourHelper::DrawColorPlate()
{
    AcGeColorArray all_colors;
    BuildColorArray( all_colors );

    CreateColorimetricPlate( AcGePoint3d( 100, 200, 0 ), 100, 1000, all_colors );
}

void ContourHelper::GetContourGEOnLayer( const CString& layer, const CString& type, AcDbObjectIdArray& objIds )
{
    AcDbObjectIdArray allObjIds;
    LayerHelper::GetAllEntitiesOnLayer( layer, allObjIds );
    if( !allObjIds.isEmpty() )
    {
        ArxDataTool::FilterEntsByType( type, allObjIds, objIds );
    }
}

void ContourHelper::PreprocessPointSet( AcGePoint3dArray& bounds, const AcGePoint3dArray& datas, AcGePoint3dArray& new_datas )
{
    ARX_PreprocessPointSet( bounds, datas, new_datas );
}