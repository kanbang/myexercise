#include "stdafx.h"

/* 测试代码(后续可能删除) */

#include "ARX_GeoAlgo.h"
#include "PolyLineJig.h"
#include "ContourHelper.h"
#include "CADDrawHelper.h"

#include <fstream>
#include "AcFStream.h"

#include "../ArxHelper/ArxUtilHelper.h"

void ReadDataFromFile( const CString& fileName, AcGePoint3dArray& datas )
{
    AcIfstream inFile( fileName );
    if( !inFile ) return;

    while( !inFile.eof() )
    {
        double x, y, z;
        inFile >> x >> y >> z;
        datas.append( AcGePoint3d( x, y, z ) );
    }
    inFile.close();
}

Acad::ErrorStatus acqHatch1()
{
    AcDbHatch* pHatch = new AcDbHatch();
    // Set hatch plane
    //
    AcGeVector3d normal( 0.0, 0.0, 1.0 );
    pHatch->setNormal( normal );
    pHatch->setElevation( 0.0 );

    // Set non associative hatch
    //
    pHatch->setAssociative( Adesk::kFalse );

    // Set hatch pattern to SolidFill type
    //
    pHatch->setPattern( AcDbHatch::kPreDefined, _T( "SOLID" ) );

    // Set hatch style to kNormal
    //
    pHatch->setHatchStyle( AcDbHatch::kNormal );

    // Construct hatch external boundary
    //
    AcGeIntArray edgeTypes;
    AcGeVoidPointerArray edgePtrs;

    AcGePoint2dArray pts;
    pts.append( AcGePoint2d( 2.0, 2.0 ) );
    pts.append( AcGePoint2d( 8.0, 2.0 ) );
    pts.append( AcGePoint2d( 8.0, 8.0 ) );
    pts.append( AcGePoint2d( 2.0, 8.0 ) );
    pts.append( AcGePoint2d( 2.0, 2.0 ) );

    int n = pts.length();
    for( int i = 0; i < n - 1; i++ )
    {
        AcGeLineSeg2d* pLine = new AcGeLineSeg2d( pts[i], pts[i + 1] );

        edgePtrs.append( pLine );
        edgeTypes.append( AcDbHatch::kLine );
    }
    // Append an external loop (rectangle) to hatch boundary

    pHatch->appendLoop( AcDbHatch::kDefault, edgePtrs, edgeTypes );

    edgeTypes.removeAll();
    edgePtrs.removeAll();
    // Construct a circle
    //
    AcGePoint2d cenPt( 5.0, 5.0 );
    double TWOPI = 2.0 * 3.1415926535897932;
    AcGeCircArc2d* cirArc = new AcGeCircArc2d();
    cirArc->setCenter( cenPt );
    cirArc->setRadius( 1.0 );
    cirArc->setAngles( 0.0, TWOPI );

    // Append an internal circular loop to hatch boundary
    //
    edgeTypes.append( AcDbHatch::kCirArc );
    edgePtrs.append( ( void* )cirArc );
    pHatch->appendLoop( AcDbHatch::kDefault, edgePtrs, edgeTypes );

    // Elaborate solid fill
    //
    pHatch->evaluateHatch();

    // Post hatch entity to database
    //
    AcDbObjectId newId;
    if( ArxUtilHelper::PostToModelSpace( pHatch ) )
    {
        pHatch->close();
    }

    return eOk;
}

Acad::ErrorStatus acqHatch2()
{
    AcDbHatch* pHatch = new AcDbHatch();
    // Set hatch plane
    //
    AcGeVector3d normal( 0.0, 0.0, 1.0 );
    pHatch->setNormal( normal );
    pHatch->setElevation( 0.0 );

    // Set non associative hatch
    //
    pHatch->setAssociative( Adesk::kFalse );

    // Set hatch pattern to SolidFill type
    //
    //pHatch->setPattern(AcDbHatch::kPreDefined, _T("SOLID"));

    // Set hatch style to kNormal
    //
    //pHatch->setHatchStyle(AcDbHatch::kNormal);

    // 设置为渐变色填充
    pHatch->setHatchObjectType( AcDbHatch::kGradientObject );

    // set gradient
    pHatch->setGradient( AcDbHatch::kPreDefinedGradient, _T( "INVSPHERICAL" ) ); // 反转球形
    pHatch->setGradientOneColorMode( false ); // 实质上都是双色
    AcCmColor* colors = new AcCmColor[2];
    colors[0].setRGB( 0, 0, 255 );
    colors[1].setRGB( 0, 0, 0 );
    float* values = new float[2];
    values[0] = 0.0f;
    values[1] = 1.0f;
    pHatch->setGradientColors( 2, colors, values );

    delete [] colors;
    delete [] values;

    // Construct hatch external boundary
    //
    AcGeIntArray edgeTypes;
    AcGeVoidPointerArray edgePtrs;

    AcGePoint2dArray pts;
    pts.append( AcGePoint2d( 2.0, 2.0 ) );
    pts.append( AcGePoint2d( 8.0, 2.0 ) );
    pts.append( AcGePoint2d( 8.0, 8.0 ) );
    pts.append( AcGePoint2d( 2.0, 8.0 ) );
    pts.append( AcGePoint2d( 2.0, 2.0 ) );

    int n = pts.length();
    for( int i = 0; i < n - 1; i++ )
    {
        AcGeLineSeg2d* pLine = new AcGeLineSeg2d( pts[i], pts[i + 1] );

        edgePtrs.append( pLine );
        edgeTypes.append( AcDbHatch::kLine );
    }
    // Append an external loop (rectangle) to hatch boundary

    pHatch->appendLoop( AcDbHatch::kDefault, edgePtrs, edgeTypes );

    edgeTypes.removeAll();
    edgePtrs.removeAll();
    // Construct a circle
    //
    AcGePoint2d cenPt( 5.0, 5.0 );
    double TWOPI = 2.0 * 3.1415926535897932;
    AcGeCircArc2d* cirArc = new AcGeCircArc2d();
    cirArc->setCenter( cenPt );
    cirArc->setRadius( 1.0 );
    cirArc->setAngles( 0.0, TWOPI );

    // Append an internal circular loop to hatch boundary
    //
    edgeTypes.append( AcDbHatch::kCirArc );
    edgePtrs.append( ( void* )cirArc );
    pHatch->appendLoop( AcDbHatch::kDefault, edgePtrs, edgeTypes );

    // Elaborate solid fill
    //
    pHatch->evaluateHatch();

    // Post hatch entity to database
    //
    AcDbObjectId newId;
    if( ArxUtilHelper::PostToModelSpace( pHatch ) )
    {
        pHatch->close();
    }

    return eOk;
}

void ContourTest()
{
    AcGePoint3dArray datas;
    //ReadDataFromFile(_T("D:\\CONTOUR(300).DAT"), datas);
    ReadDataFromFile( _T( "D:\\testsite.txt" ), datas );
    //ReadDataFromFile(_T("D:\\testdata.txt"), datas);
    //ReadDataFromFile(_T("D:\\testdata2.txt"), datas);

    AcGePoint3dArray bounds;
    double minZ, maxZ;
    ContourHelper::GetMinMaxZValue( datas, minZ, maxZ );

    acutPrintf( _T( "\n测试数据个数:%d" ), datas.length() );
    ContourHelper::DrawCountourByNum( bounds, datas, minZ, maxZ, 5 );
    //ContourHelper::DrawColorPlate();
    //acqHatch1();
    //acqHatch2();
}

void PointCaclTest()
{
    using namespace std;

    AcGePoint3d pt;
    //if(!ArxUtilHelper::PromptPt(_T("\n请选择一个坐标"), pt)) return;
    pt.x = 0;
    pt.y = 70;

    AcGePoint3dArray datas;
    ReadDataFromFile( _T( "d:\\TestSite.txt" ), datas );
    double z = ContourHelper::PointCacl( datas, pt );

    CString msg;
    msg.Format( _T( "坐标:(%.3f, %.3f)\nz值:%.3f" ), pt.x, pt.y, z );
    AfxMessageBox( msg );

    //ReadDataFromFile(_T("d:\\testdata.txt"), datas);

    //AcGePoint3dArray pts;
    //for(int i=-100;i<100;i++)
    //{
    //	for(int j=-100;j<100;j++)
    //	{
    //		pts.append(AcGePoint3d(i, j, 0));
    //	}
    //}

    //KrigingInterpolater_More(datas, pts, 4);

    //acutPrintf(_T("\n数据个数:%d,  %d"), datas.length(), pts.length());
    //ofstream ofs("d:\\out.txt");

    //int n = pts.length();
    //for(int i=0;i<n;i++)
    //{
    //		ofs<<pts[i].z<<" ";
    //		if((i+1)%100 == 0) ofs<<endl;
    //}

    //	ofs<<endl;
    //	ofs.close();

}

void GridCaclTest()
{
    AcGePoint3dArray pts;
    if( !GetPolygonByCmd( pts ) ) return;

    AcGePoint3dArray H;
    //Graham_ConvexHull_3D(pts, H); // 使用Graphm算法
    Melkman_ConvexHull_3D( pts, H ); // 使用Melkman算法

    acutPrintf( _T( "\n使用闭合多边形绘制凸包" ) );
    DrawPLines( H, true );

    // 使用AcDbExtent获取最小边界盒
    AcDbExtents ext;
    int len = H.length();
    for( int i = 0; i < len; i++ ) ext.addPoint( H[i] );

    acutPrintf( _T( "\n最小点:(%.3f, %.3f,  %.3f)" ), ext.minPoint() );
    acutPrintf( _T( "\n最大点:(%.3f, %.3f,  %.3f)" ), ext.maxPoint() );

    AcGePoint3dArray datas;
    ReadDataFromFile( _T( "TestSite.txt" ), datas );
    double z = ContourHelper::GridCacl( datas, ext.minPoint(), ext.maxPoint(), 10 );

    CString msg;
    msg.Format( _T( "计算z值:%.5f" ), z );
    AfxMessageBox( msg );
    //acutPrintf(_T("\n计算z值:%.5f"), z);
}

#include "ARX_Gas.h"
#include "ARX_ContourAlgo.h"
#include <fstream>

void writeDataToFile()
{
    AcGePoint3dArray datas;
    GetGasDatas( GCT_CONTENT, datas );

    AcGePoint3dArray bounds;
    GetBoundaryPoints( bounds );

    AcGePoint3dArray new_datas;
    ARX_PreprocessPointSet( bounds, datas, new_datas );

    using namespace std;
    ofstream outfile1( "d:\\testdata2.txt" );

    for( int i = 0; i < new_datas.length(); i++ )
    {
        outfile1 << new_datas[i].x << "\t"
                 << new_datas[i].y << "\t"
                 << new_datas[i].z << "\n";
    }
    outfile1.close();

    ofstream outfile2( "d:\\bound.txt" );

    for( int i = 0; i < bounds.length(); i++ )
    {
        outfile2 << bounds[i].x << "\t"
                 << bounds[i].y << "\t"
                 << bounds[i].z << "\n";
    }
    outfile2.close();
}
