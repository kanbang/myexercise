#include "StdAfx.h"

extern void FindAllGoafs( AcDbObjectIdArray& objIds );

/* ȫ�ֺ���(BuildGoafRegion.cpp) */
extern void GetGoafPolygon( const AcDbObjectId& objId, AcGePoint3dArray& polygon );

/* ȫ�ֺ���(DoubleLineHelper.cpp) */
extern void ExpandLinesOnPolygon( const AcDbVoidPtrArray& lines, const AcGePoint3dArray& polygon,
                                  AcGePoint3dArray& spts, AcGePoint3dArray& epts,
                                  AcGeDoubleArray& dirs, bool isInner );
extern void FilterLines( const AcDbVoidPtrArray& all_lines, AcDbVoidPtrArray& lines, bool findWS );

// ���ҹ�������ɿ��������ѹ����
void FindPressureFacility( const AcDbVoidPtrArray& lines, AcGePoint3dArray& spts, AcGePoint3dArray& epts, AcGeDoubleArray& dirs )
{
    // ���ҹ�����ֱ��
    AcDbVoidPtrArray wsLines;
    FilterLines( lines, wsLines, true );

    // �������еĲɿ���
    AcDbObjectIdArray objIds;
    FindAllGoafs( objIds );

    for( int i = 0; i < objIds.length(); i++ )
    {
        // ��ȡ�ɿ����Ķ����
        AcGePoint3dArray polygon;
        GetGoafPolygon( objIds[i], polygon );

        // ���ҹ������������չ��
        ExpandLinesOnPolygon( wsLines, polygon, spts, epts, dirs, true );
    }
}

void PrintPressFacility( const AcGePoint3dArray press_spts, const AcGePoint3dArray& press_epts, const AcGeDoubleArray& press_dirs )
{
    acutPrintf( _T( "��ӡ��������ɿ��������ѹ����: \n" ) );
    int n = press_spts.length();
    for( int i = 0; i < n; i++ )
    {
        AcGePoint3d spt = press_spts[i], ept = press_epts[i];
        double dir = press_dirs[i];
        acutPrintf( _T( "  (%.3f, %.3f) --> (%.3f, %.3f) ����: %.3f\n" ), spt.x, spt.y, ept.x, ept.y, dir );
    }
    acutPrintf( _T( "\n" ) );
}