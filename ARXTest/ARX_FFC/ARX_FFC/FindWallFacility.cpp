#include "StdAfx.h"

#include "../FFC_GE/SandWall.h"
#include "../MineGE/HelperClass.h"

extern void FindAllGoafs( AcDbObjectIdArray& objIds );

/* ȫ�ֺ���(BuildGoafRegion.cpp) */
extern void GetGoafPolygon( const AcDbObjectId& objId, AcGePoint3dArray& polygon );

/* ȫ�ֺ���(ʵ����GeoTool.cpp) */
extern void CaclLine( const AcGePoint3d& pt, double angle, double L, AcGePoint3d& spt, AcGePoint3d& ept );
extern void FindLinesInPolygon( const AcGePoint3dArray& polygon, AcGePoint3dArray& spts, AcGePoint3dArray& epts );

/* ȫ�ֺ���(TopologyHelper.cpp) */
extern void AddNewPoints( const AcGePoint3dArray& spts, const AcGePoint3dArray& epts, AcGePoint3dArray& pts );
extern void AddNewEdges( const AcGePoint3dArray& spts, const AcGePoint3dArray& epts, const AcGePoint3dArray& vertices, AcDbIntArray& edges );
extern void AddNewFace( const AcGePoint3dArray& spts, const AcGePoint3dArray& epts, const AcGePoint3dArray& vertices, const AcDbIntArray& edges, AcDbIntArray& faces );
extern void BuildPolygonFromTopology( const AcGePoint3dArray& vertices, const AcDbIntArray& edges,
                                      const AcDbIntArray& faces, const AcDbIntArray& faces_info,
                                      int k, AcGePoint3dArray& polygon );

static void GetWallLines( const AcDbObjectIdArray& objIds, AcGePoint3dArray& spts, AcGePoint3dArray& epts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    for( int i = 0; i < objIds.length(); i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk == pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) )
        {
            SandWall* pWall = SandWall::cast( pObj );
            if( pWall != 0 )
            {
                AcGePoint3d spt, ept;
                CaclLine( pWall->getInsertPt(),
                          pWall->getDirectionAngle(),
                          pWall->getLength(),
                          spt, ept );

                spts.append( spt );
                epts.append( ept );
            }
        }
    }

    actrTransactionManager->endTransaction();
}

// �����ڲɿ����ڵı�ǽ
static void FindWall( const AcGePoint3dArray& polygon, AcGePoint3dArray& spts, AcGePoint3dArray& epts )
{
    // �������еı�ǽ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "SandWall" ), objIds );

    // ��ȡ��ǽ����������
    GetWallLines( objIds, spts, epts );

    // �����ڲɿ���������ڵı�ǽ
    FindLinesInPolygon( polygon, spts, epts );
}

static void AddWallToFace( AcGePoint3dArray& vertices, AcDbIntArray& edges, const AcDbIntArray& faces, const AcDbIntArray& faces_info, int k, AcDbIntArray& walls )
{
    // ����k���������
    AcGePoint3dArray polygon;
    BuildPolygonFromTopology( vertices, edges, faces, faces_info, k, polygon );

    // �����ڶ�����ڵı�ǽ
    AcGePoint3dArray spts, epts;
    FindWall( polygon, spts, epts );

    // ����µĽڵ�ͱ�
    AddNewPoints( spts, epts, vertices );
    AddNewEdges( spts, epts, vertices, edges );

    // ������ӵı߼�¼��walls
    AddNewFace( spts, epts, vertices, edges, walls );
}

void AddWallToSplitFace( AcGePoint3dArray& vertices, AcDbIntArray& edges,
                         const AcDbIntArray& faces, const AcDbIntArray& faces_info,
                         const AcDbIntArray& goafs,
                         AcDbIntArray& split_faces,
                         AcDbIntArray& split_faces_info,
                         AcDbIntArray& split_face_edges )
{
    for( int i = 0; i < goafs.length(); i++ )
    {
        AcDbIntArray walls;
        AddWallToFace( vertices, edges, faces, faces_info, goafs[i], walls );

        if( !walls.isEmpty() )
        {
            split_faces.append( goafs[i] );
            split_faces_info.append( walls.length() );
            split_face_edges.append( walls );
        }
    }
}

void PrintSplitFace( AcDbIntArray& split_faces, AcDbIntArray& split_faces_info, AcDbIntArray& split_face_edges )
{
    for( int i = 0; i < split_faces.length(); i++ )
    {
        acutPrintf( _T( "��%d���汻�ָ�: " ), split_faces[i] );

        int s = 0;
        for( int j = 0; j < i; j++ )
        {
            s += split_faces_info[j];
        }
        int t = s + split_faces_info[i];

        acutPrintf( _T( "( " ) );
        for( int j = s; j < t; j++ )
        {
            acutPrintf( _T( "%d " ), split_face_edges[j] );
        }
        acutPrintf( _T( ")\n" ) );
    }
}