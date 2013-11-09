#include "stdafx.h"
#include "DrawCmd.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

// ͨ��ͼԪ
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

/* ȫ�ֺ���(ʵ����PromptTool.cpp) */
extern bool PromptSEPt( const CString& name, AcGePoint3d& startPt, AcGePoint3d& endPt, double& angle );
extern bool GetClosePtAndAngle( const AcDbObjectId& objId, AcGePoint3d& pt, double& angle );
extern bool PromptInsertPt( const AcDbObjectId& objId, AcGePoint3d& pt );
extern bool PromptArcPt( const CString& name, AcGePoint3d& startPt, AcGePoint3d& endPt, AcGePoint3d& thirdPt );

// ���Ʒ�������ʱ���������С����
#define MIN_LENGTH 150

static void CreateDirection( const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
    WindDirection* pDir = new WindDirection( pt, angle );
    pDir->setRelatedGE( host ); // ����ͼԪ

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pDir ) ) delete pDir;
}

static void CreateWindBridge( const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
    WindBridge* pBridge = new WindBridge( pt, angle );
    pBridge->setRelatedGE( host ); // ����ͼԪ

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pBridge ) ) delete pBridge;
}

static void CreateGate( int flag, const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
    Gate* pGate;
    switch( flag )
    {
    case 1:
        pGate = new PermanentGate( pt, angle );  // ���÷���
        break;
    case 2:
        pGate = new TemporaryGate( pt, angle );  // ��ʱ����
        break;
    case 3:
        pGate = new DoubleGate( pt, angle );    // ˫�����
        break;

    case 4:
        pGate = new BalanceGate( pt, angle ); // ƽ�����
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
        pWall = new PermanentWall( pt, angle );  // ���õ���ǽ
        break;
    case 2:
        pWall = new TemporaryWall( pt, angle );  // ��ʱ����ǽ
        break;
    default:
        pWall = 0;
    }

    if( pWall == 0 ) return;

    // �����������
    pWall->setRelatedGE( host );

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pWall ) ) delete pWall;
}

static void CreateCasement( int flag, const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
    Casement* pCasement;
    switch( flag )
    {
    case 1:
        pCasement = new WallCasement( pt, angle );       // ǽ���ڷ細
        break;
    case 2:
        pCasement = new PermanentCasement( pt, angle );  // ���õ��ڷ細
        break;
    case 3:
        pCasement = new TemporaryCasement( pt, angle );  // ��ʱ���ڷ細
        break;
    default:
        pCasement = 0;
    }

    if( pCasement == 0 ) return;

    // �����������
    pCasement->setRelatedGE( host );

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pCasement ) ) delete pCasement;
}

static void CreateWindStation( const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
    WindStation* pStation = new WindStation( pt, angle );
    // �����������
    pStation->setRelatedGE( host );

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pStation ) ) delete pStation;
}

//void DrawCmd::DrawJoint(void)
//{
//	ads_point _pt;
//	if(acedGetPoint(NULL, _T("\n��ѡ���������꣺"), _pt) != RTNORM)
//	{
//		acutPrintf(_T("\nѡ�������ʧ��"));
//		return;
//	}
//	AcGePoint3d pt(_pt[X], _pt[Y], _pt[Z]);
//
//	Joint* pJoint = new Joint(pt);
//	// ��ʼ�����ύ�����ݿ�
//	if(!ArxUtilHelper::PostToModelSpace(pJoint)) delete pJoint;
//}

void DrawCmd::DrawTunnel( void )
{
    AcGePoint3d startPt, endPt;
    double angle;
    if( !PromptSEPt( _T( "���" ), startPt, endPt, angle ) ) return;

    // 3������ͼԪ
    Tunnel* pTunnel = new Tunnel( startPt, endPt );  // ���

    // 4����ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pTunnel ) ) delete pTunnel;
}

void DrawCmd::DrawArcTunnel( void )
{
    AcGePoint3d startPt, endPt, thirdPt;
    if( !PromptArcPt( _T( "�������" ), startPt, endPt, thirdPt ) ) return;

    // 3������ͼԪ
    ArcTunnel* pArcTunnel = new ArcTunnel( startPt, endPt, thirdPt );  // �������

    // 4����ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pArcTunnel ) ) delete pArcTunnel;
}

void DrawCmd::DrawWS( void )
{
    AcGePoint3d startPt, endPt;
    double angle;
    if( !PromptSEPt( _T( "�زɹ�����" ), startPt, endPt, angle ) ) return;

    WorkSurface* pWS = new WorkSurface( startPt, endPt );

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pWS ) ) delete pWS;
}

void DrawCmd::DrawMainFan()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ�����:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "Tunnel" ), objId ) ) return;

    AcGePoint3d pt;
    if( !PromptInsertPt( objId, pt ) ) return;

    double angle;
    if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

    Fan* pFan = new MainFan( pt, angle );
    if( pFan == 0 ) return;

    pFan->setRelatedGE( objId ); // �������

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pFan ) ) delete pFan;
}

void DrawCmd::DrawLocalFan()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ�����������:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId ) ) return;

    AcDbObjectIdArray objIds;
    DrawHelper::GetTagGEById2( objId, _T( "LocalFan" ), objIds );
    if( !objIds.isEmpty() )
    {
        AfxMessageBox( _T( "�þ���������������˾ֲ�ͨ���!" ) );
        return;
    }

    AcGePoint3d pt;
    if( !ArxUtilHelper::PromptPt( _T( "\n��ָ�����ȵĲ��������:" ), pt ) ) return;

    Fan* pFan = new LocalFan( pt, 0 ); // Ŀǰ������ȵĽǶ�Ϊ0(ʵ�ʻ�ͼ�о�����û�з����)
    if( pFan == 0 ) return;

    pFan->setRelatedGE( objId ); // �������

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pFan ) ) delete pFan;
}

void DrawCmd::DrawGate( int flag )
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ�����������:" ) );
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
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ��������ɾ����桢����:" ) );
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
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ��������زɹ����桢����:" ) );
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
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ��������زɹ����桢���һ���������:" ) );
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
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ��������ɾ����桢����:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

    AcGePoint3d pt;
    if( !PromptInsertPt( objId, pt ) ) return;

    double angle;
    if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

    CreateWindBridge( objId, pt, angle ); // ����
}

void DrawCmd::DrawDirection( void )
{
    //AcDbObjectId objId = ArxUtilHelper::SelectObject(_T("��ѡ��һ��������ɾ����桢����:"));
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ�����������:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

    AcGePoint3d pt;
    if( !PromptInsertPt( objId, pt ) ) return;

    double angle;
    if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

    CreateDirection( objId, pt, angle );
}

// flag = 1 --> ��ҩ��
// flag = 2 --> ��������
// flag = 3 --> �����
void DrawCmd::DrawStorage( int flag )
{
    CString name;
    if( flag == 1 ) name = _T( "��ҩ��" );
    else if( flag == 2 ) name = _T( "��������" );
    else if( flag == 3 ) name = _T( "�����" );
    else return;

    AcGePoint3d startPt, endPt;
    double angle;
    if( !PromptSEPt( name, startPt, endPt, angle ) ) return;

    // 3������ͼԪ
    MineGE* pGE;
    switch( flag )
    {
    case 1:
        pGE = new PowderStorage( startPt, endPt );  // ��ҩ��
        break;
    case 2:
        pGE = new MachineRoom( startPt, endPt );  // ��������
        break;
    case 3:
        pGE = new ChargeRoom( startPt, endPt );    // �����
        break;
    default:
        pGE = 0;
    }

    if( pGE != 0 )
    {
        // 4����ʼ�����ύ�����ݿ�
        if( !ArxUtilHelper::PostToModelSpace( pGE ) ) delete pGE;
    }
}

//void DrawCmd::DrawJointByJig(void)
//{
//	acutPrintf(_T("\nͨ��jig��ʽ���ƽڵ����..."));
//	MineGE* pGE = DrawHelper::DrawByJig(_T("Joint"));
//
//	// ��ʼ�����ύ�����ݿ�
//	if(!ArxUtilHelper::PostToModelSpace(pGE)) delete pGE;
//}

void DrawCmd::DrawTunnelByJig( void )
{
    acutPrintf( _T( "\nͨ��jig��ʽ�����������..." ) );
    MineGE* pGE = DrawHelper::DrawByJig( _T( "Tunnel" ) );
    if( pGE == 0 ) return;

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pGE ) ) delete pGE;
}

void DrawCmd::DrawArcTunnel_ByJig( void )
{
    acutPrintf( _T( "\nͨ��jig��ʽ���ƻ����������..." ) );
    MineGE* pGE = DrawHelper::DrawByJig( _T( "ArcTunnel" ) );
    if( pGE == 0 ) return;

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pGE ) ) delete pGE;
}

void DrawCmd::DrawWSByJig( void )
{
    acutPrintf( _T( "\nͨ��jig��ʽ���ƹ��������..." ) );
    MineGE* pGE = DrawHelper::DrawByJig( _T( "WorkSurface" ) );

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pGE ) ) delete pGE;
}

//void DrawCmd::DrawCompass()
//{
//	ads_point _pt;
//	if(acedGetPoint(NULL, _T("\n��ѡ���������꣺"), _pt) != RTNORM)
//	{
//		acutPrintf(_T("\nѡ�������ʧ��"));
//		return;
//	}
//
//	double angle;
//	if(acedGetAngle(_pt, _T("\n��ָ������Ƕ�"), &angle) != RTNORM)
//	{
//		acutPrintf(_T("\n��ȡ�Ƕ�ʧ��!"));
//		return;
//	}
//
//	acutPrintf(_T("\n�Ƕ�Ϊ:%.3f"), angle);
//	AcGePoint3d pt(_pt[X], _pt[Y], _pt[Z]);
//
//	Compass* pCompass = new Compass(pt, angle);
//	// ��ʼ�����ύ�����ݿ�
//	if(!ArxUtilHelper::PostToModelSpace(pCompass)) delete pCompass;
//}

void DrawCmd::DrawChimney( void )
{
    acutPrintf( _T( "\n���Ʒ�Ͳ����..." ) );

    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ�����������:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId ) ) return;

    AcDbObjectIdArray objIds;
    DrawHelper::GetTagGEById2( objId, _T( "Chimney" ), objIds );
    if( !objIds.isEmpty() )
    {
        AfxMessageBox( _T( "�þ���������������˷�Ͳ!" ) );
        return;
    }

    AcGePoint3dArray pts;
    PolyLineJig jig;
    if( !jig.doJig( pts ) ) return;

    int len = pts.length();
    acutPrintf( _T( "\n�����:%d" ), len );
    if( len < 2 ) return;

    Chimney* pChimney = new Chimney();
    pChimney->setRelatedGE( objId ); // ������ͼԪ�����Ǿ��������

    for( int i = 0; i < len; i++ ) pChimney->addControlPoint( pts[i] );

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pChimney ) ) delete pChimney;
}