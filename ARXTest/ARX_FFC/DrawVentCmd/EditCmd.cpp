#include "stdafx.h"
#include "DrawCmd.h"

#include "../MineGE/MineGE.h"
#include "../MineGE/LinkedGE.h"
#include "../MineGE/TagGE.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

/* ȫ�ֺ���(ʵ����PromptTool.cpp) */
extern bool GetClosePtAndAngle( const AcDbObjectId& objId, AcGePoint3d& pt, double& angle );
extern bool GetSEPt( const AcDbObjectId& objId, AcGePoint3d& spt, AcGePoint3d& ept );

static bool GetInsertPt( const AcDbObjectId& objId, AcGePoint3d& pt )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    DirectionTagGE* pDir = DirectionTagGE::cast( pObj );
    if( pDir == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    pt = pDir->getInsertPt();

    actrTransactionManager->endTransaction();

    return true;
}

static void CacLineClosePt( const AcGePoint3d& spt, const AcGePoint3d& ept,
                            AcGePoint3d& pt, double& angle )
{
    // ����һ�������߶�
    AcGeLineSeg3d line( spt, ept );
    pt = line.closestPointTo( pt ); // ���������������ĵ�

    AcGeVector3d v = ept - spt;
    angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
}

static void CacArcClosePt( const AcGePoint3d& spt, const AcGePoint3d& ept, const AcGePoint3d& thirdPt,
                           AcGePoint3d& pt, double& angle )
{
    AcGeCircArc3d arc( spt, thirdPt, ept );
    AcGePoint3d cnt = arc.center();
    double radius = arc.radius();

    AcGeVector3d v = ept - cnt;
    AcGeVector3d v2 = ept - spt;
    AcGeVector3d v3 = v.crossProduct( v2 );
    int c = ( v3.z > 0 ? 1 : -1 );

    // ����Բ�������
    pt = arc.closestPointTo( pt );

    // ����Ƕ�
    v = pt - cnt;
    v.rotateBy( c * PI / 2, AcGeVector3d::kZAxis );

    angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
}

static void ResetDirectionGE( const AcDbObjectId& objId, const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }
    DirectionTagGE* pDirTag = DirectionTagGE::cast( pObj );
    if( pDirTag == 0 )
    {
        actrTransactionManager->abortTransaction();
        return;
    }
    pDirTag->setInsertPt( pt );
    pDirTag->setDirectionAngle( angle );
    pDirTag->setRelatedGE( host ); // ����ͼԪ

    actrTransactionManager->endTransaction();
}

void DrawCmd::JunctionEnclose( void )
{
    acutPrintf( _T( "\n����պ����...\n" ) );

    AcGePoint3d pt;
    if( !ArxUtilHelper::PromptPt( _T( "\nѡ����Ҫ�պϴ�������꣺" ), pt ) ) return;

    // ����պ�
    DrawHelper::LinkedGEJunctionClosure( pt );
}

void DrawCmd::ReverseDirection()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "\n��ѡ��һ����֧��ͼԪ [���/������/����]��" ) );
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

    DrawHelper::ReverseDirection( objId );
}

void DrawCmd::ReBindEdge()
{
    CString msg = _T( "��ѡ��һ����ǩͼԪ[����/���ڷ細/����ǽ/���/���վ/��������/����]:" );
    AcDbObjectId objId = ArxUtilHelper::SelectObject( msg );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "DirectionTagGE" ), objId ) ) return;

    CString msg2 = _T( "��ѡ��һ����֧ͼԪ[���/�ɾ�����/����]:" );
    AcDbObjectId host = ArxUtilHelper::SelectObject( msg2 );
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), host ) ) return;

    // ��ȡ��ǩͼԪ�Ĳ��������
    AcGePoint3d insertPt;
    if( !GetInsertPt( objId, insertPt ) ) return;

    AcGePoint3d pt;
    if( !ArxUtilHelper::PromptPt2( _T( "\n���ڷ�֧ͼԪ����ѡ��һ�������: " ), insertPt, pt ) ) return;

    double angle;
    GetClosePtAndAngle( host, pt, angle );

    // ���¹���tag
    ResetDirectionGE( objId, host, pt, angle );
}

void DrawCmd::ZoomMineGE()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ��ͼԪʵ��:" ) );
    if( objId.isNull() ) return;

    ArxEntityHelper::ZoomToEntity( objId );
}

static bool SetSEPt( const AcDbObjectId& objId, const AcGePoint3d& spt, const AcGePoint3d& ept )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    LinkedGE* pEdge = LinkedGE::cast( pObj );
    if( pEdge == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    pEdge->setSEPoint( spt, ept );
    actrTransactionManager->endTransaction();

    return true;
}

static bool GetSplitPoint( const AcGePoint3d& spt, const AcGePoint3d& ept, AcGePoint3d& pt )
{
    acedInitGet( RSG_NONULL, _T( "Dist" ) );

    ads_point _pt;
    int rt = acedGetPoint( asDblArray( spt ), _T( "��ָ���ָ������: [����(D)]" ), _pt );

    if( rt == RTKWORD )
    {
        acedInitGet( RSG_NONULL | RSG_NOZERO | RSG_NONEG, NULL );
        double d = 0;
        if( RTNORM != acedGetDist( asDblArray( spt ), _T( "���������: " ), &d ) ) return false;

        AcGeVector3d v = ept - spt;
        v.normalize();
        pt = spt + v * d;
    }
    else if( rt == RTNORM )
    {
        pt = asPnt3d( _pt );
    }
    else
    {
        return false;
    }

    return true;
}

void DrawCmd::SplitTunnel()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ�����������:" ) );
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

    AcGePoint3d spt, ept;
    if( !GetSEPt( objId, spt, ept ) )
    {
        acutPrintf( _T( "\n�޷���ȡ���/�������ʼĩ������" ) );
        return;
    }

    AcGePoint3d pt;
    if( !GetSplitPoint( spt, ept, pt ) ) return;
    //ArxUtilHelper::PromptPt(_T("��ָ���ָ�λ��: "), pt);

    if( pt == spt || pt == ept )
    {
        acutPrintf( _T( "\n���/�������ʼĩ�㲻����Ϊ�ָ��!!!" ) );
        return;
    }

    double angle;
    GetClosePtAndAngle( objId, pt, angle );
    /*AcGeLine3d L(spt, ept);
    if(!L.isOn(pt))
    {
    	acutPrintf(_T("���/������֮��ĵ㲻����Ϊ�ָ��!!!..."));
    	return;
    }*/

    // �½����
    CString type;
    DataHelper::GetTypeName( objId, type );
    LinkedGE* pEdge = LinkedGE::cast( ArxClassHelper::CreateObjectByType( type ) );
    if( pEdge == 0 ) return; // �������ʧ��

    pEdge->setSEPoint( pt, ept );
    if( !ArxUtilHelper::PostToModelSpace( pEdge ) )
    {
        // ��ӵ����ݿ�ʧ��
        delete pEdge;
    }
    else
    {
        // �ɹ���ӵ����ݿ�
        // ͬʱ�޸�ԭ���������
        SetSEPt( objId, spt, pt );
        DrawHelper::LinkedGEJunctionClosure( pt );
    }
}