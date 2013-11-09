#include "StdAfx.h"

static void CreateDBLinesFromPointPolygon( const AcGePoint3dArray& polygon, AcDbVoidPtrArray& lines )
{
    int n = polygon.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbLine* pLine =  new AcDbLine( polygon[i], polygon[( i + 1 ) % n] );
        lines.append( ( void* )pLine );
    }
}

static void CreateDBLinesFromLinePolygon( const AcGePoint3dArray& polygon, AcDbVoidPtrArray& lines )
{
    int n = polygon.length() / 2;
    for( int i = 0; i < n; i++ )
    {
        AcDbLine* pLine = new AcDbLine( polygon[2 * i], polygon[2 * i + 1] );
        lines.append( ( void* )pLine );
    }
}

AcDbRegion* BuildPointPolygonRegion( const AcGePoint3dArray& polygon )
{
    // �������ת���ɶ��AcDbLine����
    AcDbVoidPtrArray dbLines;
    CreateDBLinesFromPointPolygon( polygon, dbLines );

    // ��������region
    AcDbVoidPtrArray regions;
    if( Acad::eOk != AcDbRegion::createFromCurves( dbLines, regions ) ) return 0;

    // ɾ��
    for( int i = 0; i < dbLines.length(); i++ )
    {
        delete ( AcDbLine* )dbLines[i];
    }
    // ��������
    return ( AcDbRegion* )regions[0];
}

AcDbRegion* BuildLinePolygonRegion( const AcGePoint3dArray& line_polygon )
{
    // �������ת���ɶ��AcDbLine����
    AcDbVoidPtrArray dbLines;
    CreateDBLinesFromLinePolygon( line_polygon, dbLines );

    // ��������region
    AcDbVoidPtrArray regions;
    if( Acad::eOk != AcDbRegion::createFromCurves( dbLines, regions ) ) return 0;

    // ɾ��
    for( int i = 0; i < dbLines.length(); i++ )
    {
        delete ( AcDbLine* )dbLines[i];
    }
    // ��������
    return ( AcDbRegion* )regions[0];
}

AcDbRegion* UniteRegion( AcDbVoidPtrArray& regions )
{
    // �ϲ�����
    AcDbRegion* pRegion = ( AcDbRegion* )regions[0];
    for( int i = 1; i < regions.length(); i++ )
    {
        AcDbRegion* pOtherRegion = ( AcDbRegion* )regions[i];

        acdbAcisDeleteModelerBulletins();
        // ���򲢼�����
        pRegion->booleanOper( AcDb::kBoolUnite, pOtherRegion );

        // ɾ���ڴ�
        delete pOtherRegion;
    }

    regions.removeAll();

    return pRegion;
}

void DeletAllRegions( AcDbVoidPtrArray& regions )
{
    for( int i = 0; i < regions.length(); i++ )
    {
        AcDbRegion* pRegion = ( AcDbRegion* )regions[i];
        delete pRegion;
    }
    regions.removeAll();
}