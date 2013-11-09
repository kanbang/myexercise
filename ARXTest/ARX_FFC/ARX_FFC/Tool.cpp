#include "StdAfx.h"

#include "DoubleLine.h"

#include "../FFC_GE/Goaf.h"
#include "../FFC_GE/Obturation.h"
#include "../FFC_GE/AirLeak.h"
#include "../FFC_GE/GasPipe.h"
#include "../FFC_GE/NitrogenPipe.h"

#include "../MineGE/HelperClass.h"

/* ȫ�ֺ���(GeoTool.cpp) */
extern int FindPointOnPolygon( const AcGePoint3d& pt, const AcGePoint3dArray& polygon );

// ����ͼԪ��Ӧ����������
// ��Ҫ���ڲ����ܱչ��������
void GetHosts( const AcDbObjectIdArray& objIds, AcDbObjectIdArray& hosts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        TagGE* pTag = TagGE::cast( pObj );
        if( pTag == 0 ) continue;

        AcDbObjectId host = pTag->getRelatedGE();
        if( !host.isNull() ) hosts.append( host );
    }
    actrTransactionManager->endTransaction();
}

void FindAllLines( AcDbObjectIdArray& objIds )
{
    // �������е������������
    DrawHelper::FindMineGEs( _T( "LinkedGE" ), objIds );
}

// �������еĲɿ���
void FindAllGoafs( AcDbObjectIdArray& objIds )
{
    DrawHelper::FindMineGEs( _T( "Goaf" ), objIds );
}

void FindAllObturations( AcDbObjectIdArray& objIds )
{
    // �������е��ܱ�
    DrawHelper::FindMineGEs( _T( "Obturation" ), objIds );
}

void FindAllAirLeaks( AcDbObjectIdArray& objIds )
{
    DrawHelper::FindMineGEs( _T( "AirLeak" ), objIds );
}

void FindAllGasPipes( AcDbObjectIdArray& objIds )
{
    DrawHelper::FindMineGEs( _T( "GasPipe" ), objIds );
}

void FindAllNitrogenPipes( AcDbObjectIdArray& objIds )
{
    DrawHelper::FindMineGEs( _T( "NitrogenPipe" ), objIds );
}

void GetAirLeakPoints( AcDbObjectIdArray& objIds, AcGePoint3dArray& pts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    for( int i = 0; i < objIds.length(); i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk == pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) )
        {
            AirLeak* pAriLeak = AirLeak::cast( pObj );
            if( pAriLeak != 0 )
            {
                pts.append( pAriLeak->getInsertPt() );
            }
        }
    }

    actrTransactionManager->endTransaction();
}


void GetGasPipePoints( AcDbObjectIdArray& objIds, AcGePoint3dArray& pts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    for( int i = 0; i < objIds.length(); i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk == pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) )
        {
            GasPipe* pGasPipe = GasPipe::cast( pObj );
            if( pGasPipe != 0 )
            {
                AcGePoint3d spt, ept;
                pGasPipe->getSEPoint( spt, ept );

                pts.append( ept );
            }
        }
    }

    actrTransactionManager->endTransaction();
}

void GetNitrogenPipePoints( AcDbObjectIdArray& objIds, AcGePoint3dArray& pts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    for( int i = 0; i < objIds.length(); i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk == pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) )
        {
            NitrogenPipe* pNitrogenPipe = NitrogenPipe::cast( pObj );
            if( pNitrogenPipe != 0 )
            {
                pts.append( pNitrogenPipe->getInsertPt() );
            }
        }
    }

    actrTransactionManager->endTransaction();
}

// ��ȡ�ɿ��������
void GetGoafPolygon( const AcDbObjectId& objId, AcGePoint3dArray& polygon )
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

// �������й�����Ķ˵�����
static void GetWSLinePoints( const AcDbVoidPtrArray& lines, AcGePoint3dArray& pts )
{
    for( int i = 0; i < lines.length(); i++ )
    {
        DoubleLine* pLine = ( DoubleLine* )lines[i];
        if( pLine->isWorkSurface() )
        {
            AcGePoint3d spt, ept;
            pLine->getSEPoint( spt, ept );
            /*if(!pts.contains(spt)) */
            pts.append( spt );
            /*if(!pts.contains(ept)) */
            pts.append( ept );
        }
    }
}

static void GetGoafPoints( const AcDbObjectIdArray& objIds, AcGePoint3dArray& pts )
{
    AcGePoint3dArray goaf_pts;
    for( int i = 0; i < objIds.length(); i++ )
    {
        AcGePoint3dArray polygon;
        GetGoafPolygon( objIds[i], polygon );
        for( int j = 0; j < polygon.length(); j++ )
        {
            pts.append( polygon[j] );
        }
    }
}

// ����ֱ���빤����Ĺ�ϵ
//	0) �빤���治����
//	1) һ�����빤�������
//		+1/-1�ֱ��ʾʼ���ĩ���빤�������
//	2) ���㶼�ڹ�������
void BuildWSRelation( const AcDbVoidPtrArray& lines, AcDbIntArray& ws_relation )
{
    // �������еĹ�����˵�����
    AcGePoint3dArray ws_pts;
    GetWSLinePoints( lines, ws_pts );

    for( int i = 0; i < lines.length(); i++ )
    {
        DoubleLine* pLine = ( DoubleLine* )lines[i];

        AcGePoint3d spt, ept;
        pLine->getSEPoint( spt, ept );

        bool b1 = ws_pts.contains( spt );
        bool b2 = ws_pts.contains( ept );

        int pr = 0;
        if( b1 && b2 )
        {
            pr = 2;
        }
        else if( b1 )
        {
            pr = 1;
        }
        else if( b2 )
        {
            pr = -1;
        }
        ws_relation.append( pr );
    }
}

// ֱ����ɿ����Ĺ�ϵ
//	0) �����˵㲻�ڲɿ�����
//	1) һ���˵��ڲɿ����ϣ�����һ���ڲɿ�����
//     ע��+1/-1�ֱ�ʼ��spt��ĩ��ept�ڲɿ�����
//	2) �����˵��ڲɿ�����һ������
//     ע��+2/-2�ֱ��ʾ�����˵���һ�����ϻ�����������
static int LinePosInGoaf( const AcDbObjectIdArray& objIds, const AcGePoint3d& spt, const AcGePoint3d& ept )
{
    int pos = 0;
    for( int i = 0; i < objIds.length(); i++ )
    {
        AcGePoint3dArray polygon;
        GetGoafPolygon( objIds[i], polygon );
        int pos1 = FindPointOnPolygon( spt, polygon );
        int pos2 = FindPointOnPolygon( ept, polygon );
        if( pos1 == -1 && pos2 == -1 ) continue;

        // FindPointOnPolygon���ص��ڶ�����ϵķ�֧���
        // ���������һ������, �������1����n-1
        int d = abs( pos1 - pos2 );

        if( pos1 == -1 )
        {
            pos = -1;
        }
        else if( pos2 == -1 )
        {
            pos = 1;
        }
        else if( d == 1 || ( polygon.length() - d ) == 1 )
        {
            pos = 2;
        }
        else
        {
            pos = -2;
        }

        break;
    }
    return pos;
}

// ֱ����ɿ����Ĺ�ϵ
//	0) �����˵㲻�ڲɿ�����
//	1) һ���˵��ڲɿ����ϣ�����һ���ڲɿ�����
//     ע��+1/-1�ֱ�ʼ��spt��ĩ��ept�ڲɿ�����
//	2) �����˵��ڲɿ�����һ������
//     ע��+2/-2�ֱ��ʾ�����˵���һ�����ϻ�����������
void BuildGoafRelation( const AcDbVoidPtrArray& lines, AcDbIntArray& goaf_relation )
{
    // �������еĲɿ���
    AcDbObjectIdArray goaf_objIds;
    FindAllGoafs( goaf_objIds );

    AcGePoint3dArray goaf_pts;
    GetGoafPoints( goaf_objIds, goaf_pts );

    for( int i = 0; i < lines.length(); i++ )
    {
        DoubleLine* pLine = ( DoubleLine* )lines[i];

        AcGePoint3d spt, ept;
        pLine->getSEPoint( spt, ept );

        goaf_relation.append( LinePosInGoaf( goaf_objIds, spt, ept ) );
    }
}

// �Ƴ��빤����Ͳɿ���û�й��������
void RemoveUnrelatedLines( AcDbObjectIdArray& objIds, AcDbVoidPtrArray& lines,
                           const AcDbIntArray& ws_relation, const AcDbIntArray& goaf_relation )
{
    int n = lines.length();
    for( int i = 0; i < n; i++ )
    {
        if( ws_relation[i] == 0 && goaf_relation[i] == 0 )
        {
            delete ( DoubleLine* )lines[i];
            lines[i] = 0;
            objIds[i].setNull();  // objIds[i] = AcDbObjectId::kNull;
        }
    }
}

// ���Ҳɿ����ϵĵ�Դ
void FindPointSourceOnGoaf( AcDbObjectIdArray& objIds, AcDbVoidPtrArray& lines,
                            const AcDbIntArray& ws_relation, const AcDbIntArray& goaf_relation,
                            AcDbObjectIdArray& ob_objIds, AcGePoint3dArray& ob_pts, AcDbIntArray& ob_types )
{
    // �������е��ܱ�
    AcDbObjectIdArray all_ob_objIds;
    FindAllObturations( all_ob_objIds );

    // �����ܱչ��������
    AcDbObjectIdArray hosts;
    GetHosts( all_ob_objIds, hosts );

    for( int i = 0; i < lines.length(); i++ )
    {
        DoubleLine* pLine = ( DoubleLine* )lines[i];
        if( pLine == 0 ) continue;

        acutPrintf( _T( "\nws[%d]=%d, goaf[%d]=%d" ), i, ws_relation[i], i, goaf_relation[i] );

        // ���������
        if( ws_relation[i] == 2 ) continue;
        // �����һ���˵��빤�������������û��ȫ�����ڲɿ���
        if( ws_relation[i] != 0 && goaf_relation[i] != 2 ) continue;
        // ֱ���ڲɿ�����һ�����ϣ���û���ܱ�
        if( goaf_relation[i] == 2 && !hosts.contains( objIds[i] ) ) continue;

        // һ���˵��ڲɿ����ϣ�����һ���˵��ڲɿ�����
        if( goaf_relation[i] == 1 || goaf_relation[i] == -1 )
        {
            AcGePoint3d spt, ept;
            pLine->getSEPoint( spt, ept );

            // goaf_relation[i] = 1��ʾʼ�����ڲɿ����ϣ���ɿ�����©��
            // goaf_relation[i] = -1��ʾĩ�����ڲɿ����ϣ���ɿ�����©��
            ob_pts.append( ( goaf_relation[i] == 1 ) ? spt : ept );
            ob_objIds.append( objIds[i] );
            ob_types.append( goaf_relation[i] );
        }

        delete ( DoubleLine* )lines[i];
        lines[i] = 0;
        objIds[i].setNull();
    }
}

// �ҳ��������ƽ��������������ض�һ������
void TruncateLines( const AcDbVoidPtrArray& lines, double trlen, const AcDbIntArray& ws_relation, const AcDbIntArray& goaf_relation )
{
    // ���������빤��������
    // �������һ���˵�û�����ڲɿ����ϵ����
    // ��ws_relation[i] == goaf_relation[i] == 1��-1
    for( int i = 0; i < lines.length(); i++ )
    {
        DoubleLine* pLine = ( DoubleLine* )lines[i];
        if( pLine == 0 ) continue;
        if( pLine->isWorkSurface() ) continue;

        if( abs( ws_relation[i] ) == 1 &&
                goaf_relation[i] == ws_relation[i] )
        {
            acutPrintf( _T( "\n��%d��������ض�" ), i );

            AcGePoint3d spt, ept;
            pLine->getSEPoint( spt, ept );

            // �ضϸ����
            AcGeVector3d v = ept - spt;
            if( v.length() < trlen ) continue;

            v.normalize();
            if( ws_relation[i] == 1 )
            {
                // ʼ���ڹ�������
                pLine->setEndPoint( spt + v * trlen );
            }
            else
            {
                // ĩ���ڹ�����
                pLine->setStartPoint( ept - v * trlen );
            }
        }
    }
}