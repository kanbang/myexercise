#include "stdafx.h"
#include "DrawCmd.h"

#include "../MineGE/MineGE.h"
#include "../MineGE/LinkedGE.h"
#include "../MineGE/TagGE.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

/* 全局函数(实现在PromptTool.cpp) */
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
    // 构造一条几何线段
    AcGeLineSeg3d line( spt, ept );
    pt = line.closestPointTo( pt ); // 计算距离中线最近的点

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

    // 距离圆弧最近点
    pt = arc.closestPointTo( pt );

    // 计算角度
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
    pDirTag->setRelatedGE( host ); // 关联图元

    actrTransactionManager->endTransaction();
}

void DrawCmd::JunctionEnclose( void )
{
    acutPrintf( _T( "\n处理闭合情况...\n" ) );

    AcGePoint3d pt;
    if( !ArxUtilHelper::PromptPt( _T( "\n选择需要闭合处理的坐标：" ), pt ) ) return;

    // 处理闭合
    DrawHelper::LinkedGEJunctionClosure( pt );
}

void DrawCmd::ReverseDirection()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "\n请选择一个分支类图元 [巷道/工作面/硐室]：" ) );
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

    DrawHelper::ReverseDirection( objId );
}

void DrawCmd::ReBindEdge()
{
    CString msg = _T( "请选择一个标签图元[风门/调节风窗/挡风墙/风机/测风站/风流方向/风桥]:" );
    AcDbObjectId objId = ArxUtilHelper::SelectObject( msg );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "DirectionTagGE" ), objId ) ) return;

    CString msg2 = _T( "请选择一个分支图元[巷道/采掘工作面/硐室]:" );
    AcDbObjectId host = ArxUtilHelper::SelectObject( msg2 );
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), host ) ) return;

    // 获取标签图元的插入点坐标
    AcGePoint3d insertPt;
    if( !GetInsertPt( objId, insertPt ) ) return;

    AcGePoint3d pt;
    if( !ArxUtilHelper::PromptPt2( _T( "\n请在分支图元附近选择一个插入点: " ), insertPt, pt ) ) return;

    double angle;
    GetClosePtAndAngle( host, pt, angle );

    // 重新关联tag
    ResetDirectionGE( objId, host, pt, angle );
}

void DrawCmd::ZoomMineGE()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个图元实体:" ) );
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
    int rt = acedGetPoint( asDblArray( spt ), _T( "请指定分割点坐标: [距离(D)]" ), _pt );

    if( rt == RTKWORD )
    {
        acedInitGet( RSG_NONULL | RSG_NOZERO | RSG_NONEG, NULL );
        double d = 0;
        if( RTNORM != acedGetDist( asDblArray( spt ), _T( "请输入距离: " ), &d ) ) return false;

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
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一条巷道或工作面:" ) );
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

    AcGePoint3d spt, ept;
    if( !GetSEPt( objId, spt, ept ) )
    {
        acutPrintf( _T( "\n无法获取巷道/工作面的始末点坐标" ) );
        return;
    }

    AcGePoint3d pt;
    if( !GetSplitPoint( spt, ept, pt ) ) return;
    //ArxUtilHelper::PromptPt(_T("请指定分割位置: "), pt);

    if( pt == spt || pt == ept )
    {
        acutPrintf( _T( "\n巷道/工作面的始末点不能作为分割点!!!" ) );
        return;
    }

    double angle;
    GetClosePtAndAngle( objId, pt, angle );
    /*AcGeLine3d L(spt, ept);
    if(!L.isOn(pt))
    {
    	acutPrintf(_T("巷道/工作面之外的点不能作为分割点!!!..."));
    	return;
    }*/

    // 新建巷道
    CString type;
    DataHelper::GetTypeName( objId, type );
    LinkedGE* pEdge = LinkedGE::cast( ArxClassHelper::CreateObjectByType( type ) );
    if( pEdge == 0 ) return; // 创建巷道失败

    pEdge->setSEPoint( pt, ept );
    if( !ArxUtilHelper::PostToModelSpace( pEdge ) )
    {
        // 添加到数据库失败
        delete pEdge;
    }
    else
    {
        // 成功添加到数据库
        // 同时修改原有巷道坐标
        SetSEPt( objId, spt, pt );
        DrawHelper::LinkedGEJunctionClosure( pt );
    }
}