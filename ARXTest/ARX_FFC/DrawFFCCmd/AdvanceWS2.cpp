#include "stdafx.h"
#include "DrawCmd.h"

#include "AdvanceDlg.h"

#include "../FFC_GE/Goaf.h"

//#include "../MineGE/LinkedGE.h"
#include "../DefGE/WorkSurface.h"
#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

#include <vector>
#include <algorithm>

// Բ����
#define PI 3.1415926535897932384626433832795

extern void GetEdgeSEPts( const AcDbObjectIdArray& objIds, AcGePoint3dArray& spts, AcGePoint3dArray& epts );
extern bool GetSEPt( const AcDbObjectId& objId, AcGePoint3d& spt, AcGePoint3d& ept );
extern bool SetSEPt( const AcDbObjectId& objId, const AcGePoint3d& spt, const AcGePoint3d& ept );
extern void SetEdgeSEPts( const AcDbObjectIdArray& objIds, AcGePoint3dArray& spts, AcGePoint3dArray& epts );

extern void AutoDirection();

static void GetGoafPolygon( const AcDbObjectId& objId, AcGePoint3dArray& polygon )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk == pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        Goaf* pGoaf = Goaf::cast( pObj );
        if( pGoaf != 0 )
        {
            pGoaf->getPolygon( polygon );
        }
    }

    actrTransactionManager->endTransaction();
}

static int FindPointOnPolygon( const AcGePoint3d& pt, const AcGePoint3dArray& polygon )
{
    int pos = -1;
    int n = polygon.length();
    for( int i = 0; i < n; i++ )
    {
        AcGePoint3d spt = polygon[i], ept = polygon[( i + 1 ) % n];
        if( spt == pt )
        {
            pos = i;
            break;
        }
        else if( ept == pt )
        {
            pos = ( i + 1 ) % n;
            break;
        }
    }
    return pos;
}

static bool FindGoafByPoint( const AcGePoint3d& spt, const AcGePoint3d& ept, AcDbObjectId& objId, int& p1, int& p2 )
{
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "Goaf" ), objIds );
    if( objIds.isEmpty() ) return false;

    bool ret = false;
    for( int i = 0; i < objIds.length(); i++ )
    {
        // ��ȡ�ɿ����Ķ����
        AcGePoint3dArray polygon;
        GetGoafPolygon( objIds[i], polygon );
        int n = polygon.length();

        objId = objIds[i];
        p1 = FindPointOnPolygon( spt, polygon );
        p2 = FindPointOnPolygon( ept, polygon );
        //acutPrintf(_T("\ni=%d p1=%d p2=%d"), i, p1, p2);
        if( p1 == -1 || p2 == -1 ) continue;
        if( abs( p1 - p2 ) == 1 || abs( p1 - p2 ) == n - 1 )
        {
            ret = true;
            break;
        }
    }
    return ret;
}

static void SetNewGoafPoint( const AcDbObjectId& objId, int pos, const AcGePoint3d& pt )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk == pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        Goaf* pGoaf = Goaf::cast( pObj );
        if( pGoaf != 0 )
        {
            pGoaf->setPoint( pos, pt );
        }
    }

    actrTransactionManager->endTransaction();
}

static void CreateGoaf( const AcGePoint3dArray& polygon )
{
    Goaf* pGoaf = new Goaf();
    for( int i = 0; i < polygon.length(); i++ )
    {
        pGoaf->addPoint( polygon[i] );
    }
    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pGoaf ) ) delete pGoaf;
}

static bool GetAdvanceDist( double& dist )
{
    CAcModuleResourceOverride resourceOverride;

    AdvanceDlg dlg;
    bool ret = ( IDOK == dlg.DoModal() );
    if( ret )
    {
        dist = dlg.m_dist;
    }
    return ret;
}

static bool GetArrowDir( const AcDbObjectId& objId, bool& dir )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    WorkSurface* pWS = WorkSurface::cast( pObj );
    if( pWS == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    dir = pWS->getArrowDir();

    actrTransactionManager->endTransaction();

    return true;
}

// Ŀǰ��ȡһ������ε�˼·
// 1) �����ƶ���Ĺ���������

static void CaclPosType( const AcGePoint3d& sp1, const AcGePoint3d& ep1,
                         const AcGePoint3d& sp2, const AcGePoint3d& ep2,
                         int& type, AcGePoint3d& pt )
{
    AcGeLine3d L1( sp1, ep1 );
    AcGeLineSeg3d L2( sp2, ep2 );

    if( L1.intersectWith( L2, pt ) )
    {
        type = 0;
    }
    else
    {
        AcGeVector3d v = ep1 - sp1;
        // ��ʱ����ת90��
        v.rotateBy( 0.5 * PI, AcGeVector3d::kZAxis );

        AcGeVector3d v2 = sp2 - sp1;
        // ��������v2������v�����ϵ�ͶӰ
        // ���ͬ�����˴���0������С��0
        if( v2.dotProduct( v ) > 0 )
        {
            type = 1;
        }
        else
        {
            type = -1;
        }
    }
}

static void PartitionLines( const AcGePoint3d& spt, const AcGePoint3d& ept,
                            const AcGePoint3dArray& spts, const AcGePoint3dArray& epts,
                            AcDbIntArray& posType, AcGePoint3dArray& pts )
{
    AcGeLine3d L1( spt, ept );

    int n = spts.length();
    for( int i = 0; i < n; i++ )
    {
        AcGePoint3d pt;
        int type;
        CaclPosType( spt, ept, spts[i], epts[i], type, pt );
        posType.append( type );
        pts.append( pt );
    }
}

static void MoveWS( AcGePoint3d& spt, AcGePoint3d& ept, bool dir, double dist )
{
    int c = ( dir ? 1 : -1 );
    AcGeVector3d v = ept - spt;
    v.normalize();
    v.rotateBy( c * 0.5 * PI, AcGeVector3d::kZAxis );
    spt += v * dist;
    ept += v * dist;
}

static void RemoveTunnels( const AcDbObjectIdArray& objIds, const AcDbIntArray& posType, bool dir )
{
    // �ƽ������뻮�ַ����෴
    // ����ƽ���������ʱ�룬���ƽ���Ĺ������˳ʱ��(�Ҳ�)����Ҫɾ�������
    // ��֮�ƽ�������˳ʱ�룬���ƽ���Ĺ��������ʱ��(���)����Ҫɾ�������
    int c = ( dir ? -1 : 1 );
    int n = posType.length();
    for( int i = 0; i < n; i++ )
    {
        if( posType[i] == c )
        {
            ArxEntityHelper::EraseObject2( objIds[i], true );
        }
    }
}

static void SwapPoint( AcGePoint3d& p1, AcGePoint3d& p2 )
{
    AcGePoint3d tp = p1;
    p1 = p2;
    p2 = tp;
}

static void AdjustLinePoints( const AcGePoint3d& spt, const AcGePoint3d& ept, bool dir,
                              AcGePoint3dArray& spts, AcGePoint3dArray& epts,
                              const AcDbIntArray& posType, const AcGePoint3dArray& pts )
{
    AcGeVector3d v = ept - spt;
    int c = ( dir ? 1 : -1 );
    v.rotateBy( c * 0.5 * PI, AcGeVector3d::kZAxis );

    int n = posType.length();
    for( int i = 0; i < n; i++ )
    {
        if( posType[i] != 0 ) continue;

        // �޸��������
        AcGeVector3d v2 = epts[i] - spts[i];
        if( v2.dotProduct( v ) > 0 )
        {
            // ͬ��,���޸�ʼ������
            spts[i] = pts[i];
        }
        else
        {
            // �������޸�ĩ������
            epts[i] = pts[i];
        }
    }
}

// ��y��������
struct SortByYAxis
{
    bool operator()( const AcGePoint3d& p1, const AcGePoint3d& p2 )
    {
        return p1.y < p2.y;
    }
};

static void AdjustWS( AcGePoint3d& spt, AcGePoint3d& ept, const AcDbIntArray& posType, const AcGePoint3dArray& pts )
{
    typedef std::vector<AcGePoint3d> VPointArray;
    VPointArray vpts;

    int n = posType.length();
    for( int i = 0; i < n; i++ )
    {
        if( posType[i] == 0 )
        {
            vpts.push_back( pts[i] );
        }
    }

    // ��y��������
    std::sort( vpts.begin(), vpts.end(), SortByYAxis() );

    // �����Ծ���
    typedef std::vector<double> VDoubleArray;
    VDoubleArray dists;
    int m = vpts.size();
    for( int i = 0; i < m - 1; i++ )
    {
        dists.push_back( vpts[i + 1].y - vpts[i].y );
    }

    // ����y���������С�ĵ��
    VDoubleArray::iterator itr = std::min_element( dists.begin(), dists.end() );
    if( itr != dists.end() )
    {
        int pos = itr - dists.begin();

        AcGeVector3d v1 = ept - spt, v2 = vpts[pos + 1] - vpts[pos];
        v1.normalize();
        v2.normalize();
        if( v1.dotProduct( v2 ) < 0 )
        {
            // ����
            spt = vpts[pos + 1];
            ept = vpts[pos];
        }
        else
        {
            spt = vpts[pos];
            ept = vpts[pos + 1];
        }
    }
}

static void AdjustTunnels( const AcDbObjectIdArray& objIds,
                           const AcGePoint3dArray& spts,
                           const AcGePoint3dArray& epts,
                           const AcDbIntArray& posType )
{
    AcDbObjectIdArray new_objIds;
    AcGePoint3dArray new_spts, new_epts;
    int n = objIds.length();
    for( int i = 0; i < n; i++ )
    {
        // ֻ�����ཻ���
        if( posType[i] == 0 )
        {
            new_objIds.append( objIds[i] );
            new_spts.append( spts[i] );
            new_epts.append( epts[i] );
        }
    }

    SetEdgeSEPts( new_objIds, new_spts, new_epts );
}

static void AdjustGoaf( const AcGePoint3d& spt, const AcGePoint3d& ept,
                        const AcGePoint3d& newSpt, const AcGePoint3d& newEpt,
                        AcGePoint3dArray& path_spts, AcGePoint3dArray& path_epts )
{
    // ���ҹ����Ĳɿ���
    AcDbObjectId goaf_objId;
    int p1, p2;
    if( !FindGoafByPoint( spt, ept, goaf_objId, p1, p2 ) )
    {
        // �����µĲɿ���
        AcGePoint3dArray polygon;
        polygon.append( spt );
        polygon.append( ept );
        polygon.append( path_epts );
        polygon.append( newEpt );
        polygon.append( newSpt );
        path_spts.reverse();
        polygon.append( path_spts );
        CreateGoaf( polygon );
    }
    else
    {
        acutPrintf( _T( "\np1:%d, p2:%d" ), p1, p2 );

        AcTransaction* pTrans = actrTransactionManager->startTransaction();
        if( pTrans == 0 ) return;

        AcDbObject* pObj;
        if( Acad::eOk == pTrans->getObject( pObj, goaf_objId, AcDb::kForWrite ) )
        {
            Goaf* pGoaf = Goaf::cast( pObj );
            if( pGoaf != 0 )
            {
                pGoaf->setPoint( p1, newSpt );
                pGoaf->setPoint( p2, newEpt );
                if( p1 > p2 )
                {
                    path_spts.reverse();
                    pGoaf->insertPoints( p1, path_spts, false );
                    pGoaf->insertPoints( p2, path_epts, true );
                }
                else
                {
                    path_epts.reverse();
                    pGoaf->insertPoints( p2, path_epts, false );
                    pGoaf->insertPoints( p1, path_spts, true );
                }

                acutPrintf( _T( "\n��ӡ�����" ) );
                AcGePoint3dArray polygon;
                pGoaf->getPolygon( polygon );
                for( int i = 0; i < polygon.length(); i++ )
                {
                    acutPrintf( _T( "\n��%d����: (%.3f,%.3f)" ), i, polygon[i].x, polygon[i].y );
                }
            }
        }

        actrTransactionManager->endTransaction();
    }
}

static void LinkedGEJunctionClosure( const AcDbObjectIdArray& objIds, const AcDbIntArray& posType )
{
    int n = objIds.length();
    for( int i = 0; i < n; i++ )
    {
        if( posType[i] != 0 ) continue;
        DrawHelper::LinkedGEJunctionClosure2( objIds[i] );
    }
}

static void PrintPosType( const AcDbIntArray& posType, const AcGePoint3dArray& pts )
{
    int n = posType.length();
    for( int i = 0; i < n; i++ )
    {
        acutPrintf( _T( "\ntype=%d, ����:(%.3f, %.3f)" ), posType[i], pts[i].x, pts[i].y );
    }
}

static void PrintPath( const CString& msg, const AcGePoint3dArray& path )
{
    acutPrintf( _T( "\n%s" ), msg );
    int n = path.length();
    for( int i = 0; i < n; i++ )
    {
        acutPrintf( _T( "\n��%d��·����:(%.3f, %.3f)" ), i, path[i].x, path[i].y );
    }
}

static void FindAdvancePath( const AcGePoint3d& pt,
                             const AcGePoint3dArray& spts,
                             const AcGePoint3dArray& epts,
                             const AcDbIntArray& posType,
                             bool dir,
                             AcGePoint3dArray& path )
{
    int c = ( dir ? -1 : 1 );

    AcGeIntArray needRemovePos;
    int n = posType.length();
    for( int i = 0; i < n; i++ )
    {
        if( posType[i] == c )
        {
            needRemovePos.append( i );
        }
    }
    if( needRemovePos.isEmpty() ) return;

    AcDbIntArray skip;
    for( int i = 0; i < needRemovePos.length(); i++ )
    {
        skip.append( 0 );
    }

    // �����������
    AcGePoint3d searchPt( pt );

    bool flag = true;
    while( flag )
    {
        flag = false;
        for( int i = 0; i < needRemovePos.length(); i++ )
        {
            if( skip[i] == 1 ) continue;

            int pos = needRemovePos[i];
            if( spts[pos] == searchPt )
            {
                path.append( epts[pos] );
                searchPt = epts[pos];

                skip[i] = 1; // �Ժ�������ʱ������
                flag = true;
                break;
            }
            else if( epts[pos] == searchPt )
            {
                path.append( spts[pos] );
                searchPt = spts[pos];

                skip[i] = 1; // �Ժ�������ʱ������
                flag = true;
                break;
            }
        }
    }
}

void DrawCmd::AdvanceWS2()
{
    AcDbObjectIdArray ps_objIds;
    ArxUtilHelper::GetPickSetEntity( ps_objIds );
    if( ps_objIds.length() != 1 ) return;

    AcDbObjectId ws_objId = ps_objIds[0];
    if( !ArxUtilHelper::IsEqualType( _T( "WorkSurface" ), ws_objId ) ) return;

    // �����������
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "Tunnel" ), objIds );
    if( objIds.isEmpty() )
    {
        AfxMessageBox( _T( "������ͼ������Ӧ����һ�������桢һ�����������һ���ط����" ) );
        return;
    }

    // ��ȡ���������ʼĩ������
    AcGePoint3dArray spts, epts;
    GetEdgeSEPts( objIds, spts, epts );

    // ��ȡ�������ʼĩ����
    AcGePoint3d spt, ept;
    if( !GetSEPt( ws_objId, spt, ept ) ) return;

    // ��ȡ�������ƽ�����
    bool dir;
    if( !GetArrowDir( ws_objId, dir ) ) return;

    // ��ȡ�ƽ�����
    double dist = 0;
    if( !GetAdvanceDist( dist ) ) return;
    acutPrintf( _T( "\n�ƽ�����:%.2f" ), dist );
    if( dist < 0.01 )
    {
        AfxMessageBox( _T( "�ƽ�����������0" ) );
        return;
    }

    // �����ƽ���Ĺ�����ʼĩ������
    // ע����һ������������
    AcGePoint3d newSpt( spt ), newEpt( ept );
    MoveWS( newSpt, newEpt, dir, dist );

    // ����ֱ��
    AcDbIntArray posType;
    AcGePoint3dArray pts;
    PartitionLines( newSpt, newEpt, spts, epts, posType, pts );
    PrintPosType( posType, pts );

    // ���ҹ������ƽ��켣
    AcGePoint3dArray path_spts, path_epts;
    FindAdvancePath( spt, spts, epts, posType, dir, path_spts );
    FindAdvancePath( ept, spts, epts, posType, dir, path_epts );
    acutPrintf( _T( "\npath_spts:%d, path_epts:%d" ), path_spts.length(), path_epts.length() );

    // ��ʱ��������
    // ɾ������posType=-1�����
    RemoveTunnels( objIds, posType, dir );
    // �ƶ�posType = 0������Լ�������
    AdjustLinePoints( newSpt, newEpt, dir, spts, epts, posType, pts );
    AdjustTunnels( objIds, spts, epts, posType );
    AdjustWS( newSpt, newEpt, posType, pts );
    // �ƶ�������
    SetSEPt( ws_objId, newSpt, newEpt );

    acutPrintf( _T( "\n�ƶ�������:(%.3f, %.3f)-->(%.3f, %.3f)" ), newSpt.x, newSpt.y, newEpt.x, newEpt.y );
    // �����պ�
    LinkedGEJunctionClosure( objIds, posType );

    // ������������
    AutoDirection();

    PrintPath( _T( "ʼ��·��" ), path_spts );
    PrintPath( _T( "ĩ��·��" ), path_epts );

    // �����ɿ���
    AdjustGoaf( spt, ept, newSpt, newEpt, path_spts, path_epts );
}