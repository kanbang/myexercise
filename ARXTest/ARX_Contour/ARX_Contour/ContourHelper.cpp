#include "StdAfx.h"
#include "ContourHelper.h"

#include "ARX_GeoAlgo.h"
#include "ARX_ContourAlgo.h"
#include "ARX_DrawContour.h"

#include "CADDrawHelper.h"

#include "../ArxHelper/HelperClass.h"

// pts.length() == nDiameter*nDiameter
// ������zֵ����ʹ�ò�ֵ�㷨�����ֵ
static void DrawGrid( int nDiameter, const AcGePoint3dArray& pts, double textHeight )
{
    // ����ԭʼ����
    // ��������
    for( int i = 0; i < nDiameter; i++ )
    {
        for( int j = 0; j < nDiameter; j++ )
        {
            AcGePoint3d pt = pts[i * nDiameter + j];

            CString text;
            text.Format( _T( "%.3f" ), pt.z );

            pt.z = 0;
            DrawMText( pt, 0, text, textHeight ); // ��ע����
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
 * ��������˼·
 * ָ�����������Σ���������
 * �����п��ܴ��ڲ��������״�����������Σ��ı��ε�
 * ����ÿһ������ε�ƽ��zֵ(ͨ��ֱ�Ӳ���ƽ�����������ò�ֵ�����������ζ����zֵ��Ȼ����ƽ��)
 * ���㲻���������е�����"����"�����*ƽ��zֵ�������ܺͣ�Ȼ�������������ε����
 * �õ�������ܵ�ƽ��zֵ
 *
 * ����Ĵ���˼·��
 * 1) �����еĶ���ζ�����ɾ��Σ���Ȼÿһ��С���ε����������ͬ��
 * 2) �����񻮷ֳ�(n+1)*(n+1)����ȡÿһ��С���ε����ĵ���в�ֵ����Ϊ���ε�ƽ��ֵ
 * 3) ���ھ��ε��������ͬ�ģ����Լ򻯳��������ĵ�zֵ���ȡƽ��ֵ
 *
 * ����ܿ��ܴ������
 ------------------------------------------------
 |		-		|		-		|		-		|
 ------------------------------------------------
 |		-		|		-		|		-		|
 ------------------------------------------------
 |		-		|		-		|		-		|
 ------------------------------------------------
 |		-		|		-		|		-		|
 ------------------------------------------------
 ���ϣ�Ҫ����3*3������ʵ�ʽ����񻮷ֳ�4*4
 */
double ContourHelper::GridCacl( const AcGePoint3dArray& datas, const AcGePoint3d& minPt, const AcGePoint3d& maxPt, int nDiameter )
{
    // ÿһ��С����Ŀ�(dx)�͸�(dy)
    double dx = ( maxPt.x - minPt.x ) / ( nDiameter + 1 );
    double dy = ( maxPt.y - minPt.y ) / ( nDiameter + 1 );

    // ����߶�
    double c = 0.2;
    double textHeight = int( std::min( dx, dy ) * c );

    AcGePoint3d basePt = minPt;
    basePt.x = basePt.x + dx / 2;
    basePt.y = basePt.y + dy / 2;

    // ׼��������������
    AcGePoint3dArray pts;
    for( int i = 0; i < nDiameter; i++ )
    {
        for( int j = 0; j < nDiameter; j++ )
        {
            // ����ϵ�����½ǿ�ʼ
            double xpos = basePt.x + j * dx;
            double ypos = basePt.y + i * dy;
            pts.append( AcGePoint3d( xpos, ypos, 0 ) );
        }
    }

    // ʹ��Kriging��ֵ�㷨������ֵ������pts��zֵ
    ARX_InterpolatePoints( datas, pts );

    // ����ԭʼ����
    //DrawOrginData(datas, 4);
    //DrawGrid(nDiameter, pts, textHeight);

    // ����ƽ��ֵ
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

// ���Դ���
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

    // Ԫ������:
    int n = sizeof( rgb ) / ( sizeof( rgb[0][0] ) );
    // ��������:
    int row = sizeof( rgb ) / ( sizeof( rgb[0] ) );
    // ����:
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