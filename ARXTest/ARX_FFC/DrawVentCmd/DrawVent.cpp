#include "stdafx.h"
#include "DrawCmd.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

// 通风图元
//#include "../DefGE/Joint.h"
#include "../DefGE/Tunnel.h"
#include "../DefGE/ArcTunnel.h"
#include "../DefGE/WorkSurface.h"
#include "../DefGE/Fan.h"
#include "../DefGE/Gate.h"
#include "../DefGE/Wall.h"
#include "../DefGE/Casement.h"
#include "../DefGE/WindStation.h"
#include "../DefGE/WindDirection.h"
#include "../DefGE/WindBridge.h"

#include "../DefGE/StorageGE.h"
#include "../DefGE/Chimney.h"
//#include "../DefGE/Compass.h"

#include "PolyLineJig.h"

/* 全局函数(实现在PromptTool.cpp) */
extern bool PromptSEPt( const CString& name, AcGePoint3d& startPt, AcGePoint3d& endPt, double& angle );
extern bool GetClosePtAndAngle( const AcDbObjectId& objId, AcGePoint3d& pt, double& angle );
extern bool PromptInsertPt( const AcDbObjectId& objId, AcGePoint3d& pt );
extern bool PromptArcPt( const CString& name, AcGePoint3d& startPt, AcGePoint3d& endPt, AcGePoint3d& thirdPt );

// 绘制风流方向时，巷道的最小长度
#define MIN_LENGTH 150

static void CreateDirection( const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
    WindDirection* pDir = new WindDirection( pt, angle );
    pDir->setRelatedGE( host ); // 关联图元

    // 初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pDir ) ) delete pDir;
}

static void CreateWindBridge( const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
    WindBridge* pBridge = new WindBridge( pt, angle );
    pBridge->setRelatedGE( host ); // 关联图元

    // 初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pBridge ) ) delete pBridge;
}

static void CreateGate( int flag, const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
    Gate* pGate;
    switch( flag )
    {
    case 1:
        pGate = new PermanentGate( pt, angle );  // 永久风门
        break;
    case 2:
        pGate = new TemporaryGate( pt, angle );  // 临时风门
        break;
    case 3:
        pGate = new DoubleGate( pt, angle );    // 双向风门
        break;

    case 4:
        pGate = new BalanceGate( pt, angle ); // 平衡风门
        break;

    default:
        pGate = 0;
    }

    if( pGate == 0 ) return;

    pGate->setRelatedGE( host );

    if( !ArxUtilHelper::PostToModelSpace( pGate ) ) delete pGate;
}

static void CreateWall( int flag, const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
    Wall* pWall;
    switch( flag )
    {
    case 1:
        pWall = new PermanentWall( pt, angle );  // 永久挡风墙
        break;
    case 2:
        pWall = new TemporaryWall( pt, angle );  // 临时挡风墙
        break;
    default:
        pWall = 0;
    }

    if( pWall == 0 ) return;

    // 关联到巷道上
    pWall->setRelatedGE( host );

    // 初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pWall ) ) delete pWall;
}

static void CreateCasement( int flag, const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
    Casement* pCasement;
    switch( flag )
    {
    case 1:
        pCasement = new WallCasement( pt, angle );       // 墙调节风窗
        break;
    case 2:
        pCasement = new PermanentCasement( pt, angle );  // 永久调节风窗
        break;
    case 3:
        pCasement = new TemporaryCasement( pt, angle );  // 临时调节风窗
        break;
    default:
        pCasement = 0;
    }

    if( pCasement == 0 ) return;

    // 关联到巷道上
    pCasement->setRelatedGE( host );

    // 初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pCasement ) ) delete pCasement;
}

static void CreateWindStation( const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
    WindStation* pStation = new WindStation( pt, angle );
    // 关联到巷道上
    pStation->setRelatedGE( host );

    // 初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pStation ) ) delete pStation;
}

//void DrawCmd::DrawJoint(void)
//{
//	ads_point _pt;
//	if(acedGetPoint(NULL, _T("\n请选择插入点坐标："), _pt) != RTNORM)
//	{
//		acutPrintf(_T("\n选择点坐标失败"));
//		return;
//	}
//	AcGePoint3d pt(_pt[X], _pt[Y], _pt[Z]);
//
//	Joint* pJoint = new Joint(pt);
//	// 初始化并提交到数据库
//	if(!ArxUtilHelper::PostToModelSpace(pJoint)) delete pJoint;
//}

void DrawCmd::DrawTunnel( void )
{
    AcGePoint3d startPt, endPt;
    double angle;
    if( !PromptSEPt( _T( "巷道" ), startPt, endPt, angle ) ) return;

    // 3、创建图元
    Tunnel* pTunnel = new Tunnel( startPt, endPt );  // 巷道

    // 4、初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pTunnel ) ) delete pTunnel;
}

void DrawCmd::DrawArcTunnel( void )
{
    AcGePoint3d startPt, endPt, thirdPt;
    if( !PromptArcPt( _T( "弧线巷道" ), startPt, endPt, thirdPt ) ) return;

    // 3、创建图元
    ArcTunnel* pArcTunnel = new ArcTunnel( startPt, endPt, thirdPt );  // 弧线巷道

    // 4、初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pArcTunnel ) ) delete pArcTunnel;
}

void DrawCmd::DrawWS( void )
{
    AcGePoint3d startPt, endPt;
    double angle;
    if( !PromptSEPt( _T( "回采工作面" ), startPt, endPt, angle ) ) return;

    WorkSurface* pWS = new WorkSurface( startPt, endPt );

    // 初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pWS ) ) delete pWS;
}

void DrawCmd::DrawMainFan()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一条巷道:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "Tunnel" ), objId ) ) return;

    AcGePoint3d pt;
    if( !PromptInsertPt( objId, pt ) ) return;

    double angle;
    if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

    Fan* pFan = new MainFan( pt, angle );
    if( pFan == 0 ) return;

    pFan->setRelatedGE( objId ); // 关联巷道

    // 初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pFan ) ) delete pFan;
}

void DrawCmd::DrawLocalFan()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个掘进工作面:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId ) ) return;

    AcDbObjectIdArray objIds;
    DrawHelper::GetTagGEById2( objId, _T( "LocalFan" ), objIds );
    if( !objIds.isEmpty() )
    {
        AfxMessageBox( _T( "该掘进工作面已设置了局部通风机!" ) );
        return;
    }

    AcGePoint3d pt;
    if( !ArxUtilHelper::PromptPt( _T( "\n请指定局扇的插入点坐标:" ), pt ) ) return;

    Fan* pFan = new LocalFan( pt, 0 ); // 目前假设局扇的角度为0(实际绘图中局扇是没有方向的)
    if( pFan == 0 ) return;

    pFan->setRelatedGE( objId ); // 关联巷道

    // 初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pFan ) ) delete pFan;
}

void DrawCmd::DrawGate( int flag )
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个巷道、硐室:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "Tunnel" ), objId ) &&
            !ArxUtilHelper::IsEqualType( _T( "StorageGE" ), objId ) ) return;

    AcGePoint3d pt;
    if( !PromptInsertPt( objId, pt ) ) return;

    double angle;
    if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

    CreateGate( flag, objId, pt, angle );
}

void DrawCmd::DrawWall( int flag )
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个巷道、采掘工作面、硐室:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

    AcGePoint3d pt;
    if( !PromptInsertPt( objId, pt ) ) return;

    double angle;
    if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

    CreateWall( flag, objId, pt, angle );
}

void DrawCmd::DrawCasement( int flag )
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个巷道、回采工作面、硐室:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;
    if( ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId ) ) return;

    AcGePoint3d pt;
    if( !PromptInsertPt( objId, pt ) ) return;

    double angle;
    if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

    CreateCasement( flag, objId, pt, angle );
}

void DrawCmd::DrawWindStation()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个巷道、回采工作面、硐室或掘进工作面:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

    AcGePoint3d pt;
    if( !PromptInsertPt( objId, pt ) ) return;

    double angle;
    if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

    CreateWindStation( objId, pt, angle );
}

void DrawCmd::DrawWindBridge()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个巷道、采掘工作面、硐室:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

    AcGePoint3d pt;
    if( !PromptInsertPt( objId, pt ) ) return;

    double angle;
    if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

    CreateWindBridge( objId, pt, angle ); // 风桥
}

void DrawCmd::DrawDirection( void )
{
    //AcDbObjectId objId = ArxUtilHelper::SelectObject(_T("请选择一个巷道、采掘工作面、硐室:"));
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个巷道或工作面:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

    AcGePoint3d pt;
    if( !PromptInsertPt( objId, pt ) ) return;

    double angle;
    if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

    CreateDirection( objId, pt, angle );
}

// flag = 1 --> 火药库
// flag = 2 --> 机电硐室
// flag = 3 --> 充电室
void DrawCmd::DrawStorage( int flag )
{
    CString name;
    if( flag == 1 ) name = _T( "火药库" );
    else if( flag == 2 ) name = _T( "机电硐室" );
    else if( flag == 3 ) name = _T( "充电室" );
    else return;

    AcGePoint3d startPt, endPt;
    double angle;
    if( !PromptSEPt( name, startPt, endPt, angle ) ) return;

    // 3、创建图元
    MineGE* pGE;
    switch( flag )
    {
    case 1:
        pGE = new PowderStorage( startPt, endPt );  // 火药库
        break;
    case 2:
        pGE = new MachineRoom( startPt, endPt );  // 机电硐室
        break;
    case 3:
        pGE = new ChargeRoom( startPt, endPt );    // 充电室
        break;
    default:
        pGE = 0;
    }

    if( pGE != 0 )
    {
        // 4、初始化并提交到数据库
        if( !ArxUtilHelper::PostToModelSpace( pGE ) ) delete pGE;
    }
}

//void DrawCmd::DrawJointByJig(void)
//{
//	acutPrintf(_T("\n通过jig方式绘制节点测试..."));
//	MineGE* pGE = DrawHelper::DrawByJig(_T("Joint"));
//
//	// 初始化并提交到数据库
//	if(!ArxUtilHelper::PostToModelSpace(pGE)) delete pGE;
//}

void DrawCmd::DrawTunnelByJig( void )
{
    acutPrintf( _T( "\n通过jig方式绘制巷道测试..." ) );
    MineGE* pGE = DrawHelper::DrawByJig( _T( "Tunnel" ) );
    if( pGE == 0 ) return;

    // 初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pGE ) ) delete pGE;
}

void DrawCmd::DrawArcTunnel_ByJig( void )
{
    acutPrintf( _T( "\n通过jig方式绘制弧线巷道测试..." ) );
    MineGE* pGE = DrawHelper::DrawByJig( _T( "ArcTunnel" ) );
    if( pGE == 0 ) return;

    // 初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pGE ) ) delete pGE;
}

void DrawCmd::DrawWSByJig( void )
{
    acutPrintf( _T( "\n通过jig方式绘制工作面测试..." ) );
    MineGE* pGE = DrawHelper::DrawByJig( _T( "WorkSurface" ) );

    // 初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pGE ) ) delete pGE;
}

//void DrawCmd::DrawCompass()
//{
//	ads_point _pt;
//	if(acedGetPoint(NULL, _T("\n请选择插入点坐标："), _pt) != RTNORM)
//	{
//		acutPrintf(_T("\n选择点坐标失败"));
//		return;
//	}
//
//	double angle;
//	if(acedGetAngle(_pt, _T("\n请指定方向角度"), &angle) != RTNORM)
//	{
//		acutPrintf(_T("\n获取角度失败!"));
//		return;
//	}
//
//	acutPrintf(_T("\n角度为:%.3f"), angle);
//	AcGePoint3d pt(_pt[X], _pt[Y], _pt[Z]);
//
//	Compass* pCompass = new Compass(pt, angle);
//	// 初始化并提交到数据库
//	if(!ArxUtilHelper::PostToModelSpace(pCompass)) delete pCompass;
//}

void DrawCmd::DrawChimney( void )
{
    acutPrintf( _T( "\n绘制风筒测试..." ) );

    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个掘进工作面:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId ) ) return;

    AcDbObjectIdArray objIds;
    DrawHelper::GetTagGEById2( objId, _T( "Chimney" ), objIds );
    if( !objIds.isEmpty() )
    {
        AfxMessageBox( _T( "该掘进工作面已设置了风筒!" ) );
        return;
    }

    AcGePoint3dArray pts;
    PolyLineJig jig;
    if( !jig.doJig( pts ) ) return;

    int len = pts.length();
    acutPrintf( _T( "\n点个数:%d" ), len );
    if( len < 2 ) return;

    Chimney* pChimney = new Chimney();
    pChimney->setRelatedGE( objId ); // 关联的图元必须是掘进工作面

    for( int i = 0; i < len; i++ ) pChimney->addControlPoint( pts[i] );

    // 初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pChimney ) ) delete pChimney;
}