#include "stdafx.h"
#include "DrawCmd.h"

#include "../ArxHelper/HelperClass.h"

#include "../GasGE/GOP.h"
#include "../GasGE/GCP.h"
#include "../GasGE/GPP.h"
#include "../GasGE/PPP.h"
#include "../GasGE/DCI.h"
#include "../GasGE/GFP.h"
#include "../GasGE/CDH.h"

void DrawCmd::DrawGOP()
{
    AcGePoint3d pt;
    if( !ArxUtilHelper::PromptPt( _T( "\n��ѡ���������꣺" ), pt ) )
    {
        acutPrintf( _T( "\nѡ�������ʧ��" ) );
        return;
    }

    int flag;
    if( RTNORM != acedGetInt( _T( "\n��ѡ��ͻ��������:[1)С�� 2)���� 3)���� 4) �ش���]: " ), &flag ) )
    {
        return;
    }

    GOP* pGOP = 0;
    switch( flag )
    {
    case 1:
        pGOP = new SmallGOP( pt );
        break;

    case 2:
        pGOP = new MidGOP( pt );
        break;

    case 3:
        pGOP = new LargeGOP( pt );
        break;

    case 4:
        pGOP = new LargerGOP( pt );
        break;
    }
    if( pGOP == 0 ) return;

    if( !ArxUtilHelper::PostToModelSpace( pGOP ) ) delete pGOP;
}

void DrawCmd::DrawGCP()
{
    ads_point _pt;
    if( acedGetPoint( NULL, _T( "\n��ѡ���������꣺" ), _pt ) != RTNORM )
    {
        acutPrintf( _T( "\nѡ�������ʧ��" ) );
        return;
    }
    AcGePoint3d pt( _pt[X], _pt[Y], _pt[Z] );

    GCP* pGCP = new GCP( pt );
    if( !ArxUtilHelper::PostToModelSpace( pGCP ) ) delete pGCP;
}

void DrawCmd::DrawGPP()
{
    ads_point _pt;
    if( acedGetPoint( NULL, _T( "\n��ѡ���������꣺" ), _pt ) != RTNORM )
    {
        acutPrintf( _T( "\nѡ�������ʧ��" ) );
        return;
    }
    AcGePoint3d pt( _pt[X], _pt[Y], _pt[Z] );

    GPP* pGPP = new GPP( pt );
    if( !ArxUtilHelper::PostToModelSpace( pGPP ) ) delete pGPP;
}

void DrawCmd::DrawPPP()
{
    ads_point _pt;
    if( acedGetPoint( NULL, _T( "\n��ѡ���������꣺" ), _pt ) != RTNORM )
    {
        acutPrintf( _T( "\nѡ�������ʧ��" ) );
        return;
    }
    AcGePoint3d pt( _pt[X], _pt[Y], _pt[Z] );

    PPP* pPPP = new PPP( pt );
    if( !ArxUtilHelper::PostToModelSpace( pPPP ) ) delete pPPP;
}

// flag = 1 --> ú��Ԥ��ָ��
// flag = 2 --> ������Ԥ��ָ��1
// flag = 3 --> ������Ԥ��ָ��2
void DrawCmd::DrawDCI( int flag )
{
    ads_point _pt;
    if( acedGetPoint( NULL, _T( "\n��ѡ���������꣺" ), _pt ) != RTNORM )
    {
        acutPrintf( _T( "\nѡ�������ʧ��" ) );
        return;
    }
    AcGePoint3d pt( _pt[X], _pt[Y], _pt[Z] );

    DCI* pDCI = 0;
    switch( flag )
    {
    case 1:
        pDCI = new CSDCI( pt );
        break;

    case 2:
        pDCI = new WDCI1( pt );
        break;

    case 3:
        pDCI = new WDCI2( pt );
        break;

    }
    if( pDCI == 0 ) return;

    if( !ArxUtilHelper::PostToModelSpace( pDCI ) ) delete pDCI;
}

// flag = 1 --> ��������˹ӿ��
// flag = 2 --> �����������˹ӿ��
void DrawCmd::DrawGFP( int flag )
{
    ads_point _pt;
    if( acedGetPoint( NULL, _T( "\n��ѡ���������꣺" ), _pt ) != RTNORM )
    {
        acutPrintf( _T( "\nѡ�������ʧ��" ) );
        return;
    }
    AcGePoint3d pt( _pt[X], _pt[Y], _pt[Z] );

    GFP* pGFP = 0;
    switch( flag )
    {
    case 1:
        pGFP = new WGFP( pt );
        break;

    case 2:
        pGFP = new TGFP( pt );
        break;
    }
    if( pGFP == 0 ) return;

    if( !ArxUtilHelper::PostToModelSpace( pGFP ) ) delete pGFP;
}

void DrawCmd::DrawCDH()
{
    ads_point _pt;
    if( acedGetPoint( NULL, _T( "\n��ѡ���������꣺" ), _pt ) != RTNORM )
    {
        acutPrintf( _T( "\nѡ�������ʧ��" ) );
        return;
    }
    AcGePoint3d pt( _pt[X], _pt[Y], _pt[Z] );

    CDH* pCDH = new CDH( pt );
    if( !ArxUtilHelper::PostToModelSpace( pCDH ) ) delete pCDH;
}

