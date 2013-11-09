#include "stdafx.h"

#include "LinkedGE.h"
#include <algorithm>
#include <vector>

// �������ӵ�junctionPt�����ķ�֧ͼԪ(�������ε�ͼԪ)
static void FindLinesByPoint( const AcGePoint3d& junctionPt, AcDbObjectIdArray& objIds )
{
    AcDbBlockTable* pBlkTbl;
    acdbHostApplicationServices()->workingDatabase()->getSymbolTable( pBlkTbl, AcDb::kForRead );

    AcDbBlockTableRecord* pBlkTblRcd;
    pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();

    AcDbBlockTableRecordIterator* pBlkTblRcdItr;
    pBlkTblRcd->newIterator( pBlkTblRcdItr );

    for ( pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step() )
    {
        // ������transaction�ķ�������LinkedGE��
        // �ȼ����ų���ǰ������write״̬�༭��LinkedGE
        // ��Ҫ(***)
        AcDbEntity* pEnt = 0;
        if( Acad::eOk != pBlkTblRcdItr->getEntity( pEnt, AcDb::kForRead ) ) continue;

        LinkedGE* pEdge = LinkedGE::cast( pEnt );
        if( pEdge != 0 )
        {
            AcGePoint3d startPt, endPt;
            pEdge->getSEPoint( startPt, endPt );
            if( startPt == junctionPt || endPt == junctionPt )
            {
                objIds.append( pEdge->objectId() );
            }
        }
        pEnt->close();
    }
    delete pBlkTblRcdItr;
    pBlkTblRcd->close();
}

struct JunctionEdgeInfo
{
    AcDbObjectId objId;             // ͼԪ��id
    bool startOrEnd;                // ���ӵ���ʼ�㻹��ĩ��
    AcGeVector3d angle;             // �������췽������

    bool operator==( const JunctionEdgeInfo& info )
    {
        return ( objId == info.objId ) &&
               ( startOrEnd == info.startOrEnd ) &&
               ( angle == info.angle );
    }
};

typedef std::vector<JunctionEdgeInfo> EdgeInfo;
typedef EdgeInfo::iterator EdgeInfoIter;

static LinkedGE* OpenEdge2( AcTransaction* pTrans, const AcDbObjectId& objId, AcDb::OpenMode openMode )
{
    AcDbObject* pObj = 0;
    if( Acad::eOk != pTrans->getObject( pObj, objId, openMode ) ) return 0;

    LinkedGE* pEdge = LinkedGE::cast( pObj );
    return pEdge;
}

static void BuildJunctionEdgeInfo( const AcDbObjectIdArray& objIds,
                                   const AcGePoint3d& junctionPt,
                                   EdgeInfo& ges )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int n = objIds.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbObjectId objId = objIds.at( i );

        JunctionEdgeInfo info = {objId, true, AcGeVector3d::kIdentity};

        LinkedGE* pEdge = OpenEdge2( pTrans, objId, AcDb::kForRead );
        if( pEdge == 0 ) continue;

        AcGePoint3d startPt, endPt;
        pEdge->getSEPoint( startPt, endPt );
        if( startPt == junctionPt )
        {
            info.startOrEnd = true;
            info.angle = pEdge->getStartPointInExtendAngle();
        }
        else if( endPt == junctionPt )
        {
            info.startOrEnd = false;
            info.angle = pEdge->getEndPointInExtendAngle();
        }

        if( info.angle.isZeroLength() ) continue; // ��������ʾ������պ�
        //acutPrintf(_T("\nid:%d\tangle:%.3f\tdraw:%s"),
        //			info.objId,
        //			info.angle.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis),
        //			pEdge->getCurrentDraw());

        ges.push_back( info );
    }
    actrTransactionManager->endTransaction();
}

struct SortEdgeByAngle
{
    bool operator()( const JunctionEdgeInfo& info1, const JunctionEdgeInfo& info2 )
    {
        double angle1 = info1.angle.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
        double angle2 = info2.angle.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
        return angle1 < angle2;
    }
};

static void SortJunctionEdge( EdgeInfo& ges )
{
    // ���սǶȴ�С������ʱ�룩
    std::sort( ges.begin(), ges.end(), SortEdgeByAngle() ); // ����ʹ��list��ֻ��ʹ���������
}

// �µļ��㷽��
static AcGeVector3d CaclAverageVector2( const AcGeVector3d& v1, double w1, const AcGeVector3d& v2, double w2 )
{
    // ��v1->v2��ת��
    double angle = v2.angleTo( v1, -AcGeVector3d::kZAxis );

    // ��������v1�ĳ���
    double L1 = 0.5 * w1 / sin( angle );

    // ��������L2�ĳ���
    double L2 = 0.5 * w2 / sin( angle );

    // �������Ľ�����
    AcGeVector3d v3 = v1 * L2 + v2 * L1;

    // ������"�н�"����PIʱ�������Ƿ����
    //if(angle >= PI)
    //{
    //	//acutPrintf(_T("\n�н�:%.3f   ��������..."), angle);
    //	v3.negate();
    //}

    return v3;
}

// ԭ���ļ��㷽��
static AcGeVector3d CaclAverageVector3( const AcGeVector3d& v1, const AcGeVector3d& v2 )
{
    double angle = v2.angleTo( v1, -AcGeVector3d::kZAxis );

    // �������Ľ�����
    AcGeVector3d v3 = v1 + v2;

    // ������"�н�"����PIʱ�������Ƿ����
    if( angle > 3.1415926535897932384626433832795 )
    {
        //acutPrintf(_T("\n�н�:%.3f   ��������..."), angle);
        v3.negate();
    }

    return v3;
}

static void DealWithBoundary2( AcTransaction* pTrans,
                               const JunctionEdgeInfo& info,
                               const AcGePoint3d& junctionPt,
                               const AcGeVector3d& v )
{
    LinkedGE* pEdge = OpenEdge2( pTrans, info.objId, AcDb::kForWrite ); // ��writeģʽ��

    if( pEdge == 0 ) return;

    //acutPrintf(_T("\n���߽Ƕ�:%.3f"), v.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis));
    AcGeRay3d boundaryLine( junctionPt, v );
    if( info.startOrEnd )
    {
        //acutPrintf(_T("\n����id:%d��ʼ�˱߽�..."), pEdge->objectId());
        pEdge->dealWithStartPointBoundary( boundaryLine );
    }
    else
    {
        //acutPrintf(_T("\n����id:%d��ĩ�˱߽�..."), pEdge->objectId());
        pEdge->dealWithEndPointBoundary( boundaryLine );
    }
}

// ������Ҫ2��Ԫ�ز�����ȷ�ıպ�
static void EdgeJunctionClosureImpl( const AcGePoint3d& junctionPt, EdgeInfo& ges )
{
    //acutPrintf(_T("\n�����е�Ԫ�ظ���:%d"), ges.size());
    if( ges.size() == 1 )
    {
        ges.push_back( ges.front() );
    }

    // �ѵ�1����ӵ�ĩβ������ѭ��
    ges.push_back( ges.front() );

    // ��¼ÿ�δ���պϵĵ�ǰ����
    AcGeVector3d v3 = ges.front().angle;
    v3.rotateBy( PI / 2, AcGeVector3d::kZAxis );

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    for( EdgeInfoIter itr = ges.begin(); itr != ges.end(); itr++ )
    {
        EdgeInfoIter itr2 = itr + 1;
        if( itr2 == ges.end() ) break;

        //acutPrintf(_T("\n���1�Ƕ�:%.3f, ���2�Ƕ�:%.3f"),
        //	itr->angle.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis),
        //	itr2->angle.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis));

        AcGeVector3d cv = itr->angle.crossProduct( itr2->angle ); // ���(����нǵ�=0��PI��������=0)
        //if(cv.length() > 0.001)
        if( !cv.isZeroLength() )
        {
            //acutPrintf(_T("\n��˳���=%.3f"), cv.length());
            //v3 = CaclAverageVector3(itr->angle, itr2->angle);
            v3 = CaclAverageVector2( itr->angle, 1, itr2->angle, 1 );
        }
        else
        {
            // ƽ��(�н�=0��PI)
            //acutPrintf(_T("\n���=0"));
            v3.negate();
        }

        DealWithBoundary2( pTrans, *itr, junctionPt, v3 );
        DealWithBoundary2( pTrans, *itr2, junctionPt, v3 );
    }
    actrTransactionManager->endTransaction();
}

static void UpdateEdge( const AcDbObjectIdArray& objIds )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int n = objIds.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbObjectId objId = objIds.at( i );

        LinkedGE* pEdge = OpenEdge2( pTrans, objId, AcDb::kForWrite );
        pEdge->recordGraphicsModified( true ); // ��ǩͼ���Ѿ��޸ģ���Ҫ����ͼ��
    }
    actrTransactionManager->endTransaction();
}

void LinkedGEJunctionClosure_Helper( const AcGePoint3d& junctionPt )
{
    //acutPrintf(_T("\n�պϵ�:(%.3f, %.3f)"), junctionPt.x, junctionPt.y);
    AcDbObjectIdArray objIds;
    FindLinesByPoint( junctionPt, objIds );

    int len = objIds.length();
    //acutPrintf(_T("\n�ҵ�Ҫ����պϵķ�֧����:%d"), len);
    if( len < 1 ) return;

    EdgeInfo ges;
    BuildJunctionEdgeInfo( objIds, junctionPt, ges );          // ����junctionPt���괦�Ĺ�����֧��ͼԪ
    if( ges.size() > 0 )
    {
        SortJunctionEdge( ges );                          // ����˳�򣬰��Ƕȴ�С��ʱ������
        EdgeJunctionClosureImpl( junctionPt, ges );            // �����޸���������Ĳ���
    }
    UpdateEdge( objIds );                                          // ����ʵ��
}