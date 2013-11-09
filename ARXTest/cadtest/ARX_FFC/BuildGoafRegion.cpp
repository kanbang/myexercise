#include "StdAfx.h"

/* ȫ�ֺ���(RegionHelper.cpp) */
extern AcDbRegion* BuildPointPolygonRegion( const AcGePoint3dArray& polygon );

/* ȫ�ֺ���(DoubleLineHelper.cpp) */
extern void AdjustPointPolygon( const AcDbVoidPtrArray& lines, const AcGePoint3dArray& polygon, AcGePoint3dArray& ext_polygon, bool isInner );

/* ȫ�ֺ���(Tool.cpp) */
extern void FindAllGoafs( AcDbObjectIdArray& objIds );
extern void GetGoafPolygon( const AcDbObjectId& objId, AcGePoint3dArray& polygon );

// �����ɿ�������(�����ж���ɿ���)
void BuildGoafRegion( const AcDbVoidPtrArray& lines, AcDbVoidPtrArray& regions )
{
    // �������еĲɿ���
    AcDbObjectIdArray objIds;
    FindAllGoafs( objIds );

    for( int i = 0; i < objIds.length(); i++ )
    {
        // ��ȡ�ɿ����Ķ����
        AcGePoint3dArray polygon;
        GetGoafPolygon( objIds[i], polygon );

        // �����ɿ��������(������չ)
        AcGePoint3dArray ext_polygon;
        AdjustPointPolygon( lines, polygon, ext_polygon, true );

        // ��������
        AcDbRegion* pRegion = BuildPointPolygonRegion( ext_polygon );
        if( pRegion != 0 )
        {
            regions.append( pRegion );
        }
    }
}