#include "StdAfx.h"
#include "resource.h"

#include "../MineGE/HelperClass.h"

#include "DrawCmd.h"

// ����ע���������
#ifndef DRAW_GAS_CMD_SERVICE_NAME
#define DRAW_GAS_CMD_SERVICE_NAME _T("DRAWGASCMD_SERVICE_NAME")
#endif

/* ȫ�ֺ���(ʵ����ImportBlock.cpp) */
extern void UpdateDwgBlock();

class CDrawGasCmdApp : public AcRxArxApp
{
public:
    CDrawGasCmdApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // ע�����
        acrxRegisterService( DRAW_GAS_CMD_SERVICE_NAME );

        acutPrintf( _T( "\nDrawGasCmd::On_kLoadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // ɾ������
        delete acrxServiceDictionary->remove( DRAW_GAS_CMD_SERVICE_NAME );

        acutPrintf( _T( "\nDrawGasCmd::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt ) ;

        // �ӵ�ǰarxģ������·���¶�ȡ���µ���˹����ͼ�鶨��
        UpdateDwgBlock();

        acutPrintf( _T( "\nDrawGasCmd::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg ( pkt ) ;

        acutPrintf( _T( "\nDrawGasCmd::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    // ����С��ͻ����
    static void JL_DrawGOP()
    {
        //DrawCmd::DrawGOP();
        DrawCmd::DrawGOP2();
    }

    // ������˹������
    static void JL_DrawGCP()
    {
        //DrawCmd::DrawGCP();
        DrawCmd::DrawGCP2();
    }

    // ������˹ѹ����
    static void JL_DrawGPP()
    {
        //DrawCmd::DrawGPP();
        DrawCmd::DrawGPP2();
    }

    // ���ƶ��������
    static void JL_DrawPPP()
    {
        //DrawCmd::DrawPPP();
        DrawCmd::DrawPPP2();
    }

    // ú��ͻ��Σ����Ԥ��ָ��
    static void JL_DrawCSDCI()
    {
        //DrawCmd::DrawDCI(1);
        DrawCmd::DrawDCI2( 1 );
    }

    // ������ͻ��Σ����Ԥ��ָ��1
    static void JL_DrawWDCI1()
    {
        //DrawCmd::DrawDCI(2);
        DrawCmd::DrawDCI2( 2 );
    }

    // ������ͻ��Σ����Ԥ��ָ��2
    static void JL_DrawWDCI2()
    {
        //DrawCmd::DrawDCI(3);
        DrawCmd::DrawDCI2( 3 );
    }

    // ��������˹ӿ��
    static void JL_DrawWGFP()
    {
        //DrawCmd::DrawGFP(1);
        DrawCmd::DrawGFP2( 1 );
    }

    // �����������˹ӿ��
    static void JL_DrawTGFP()
    {
        //DrawCmd::DrawGFP(2);
        DrawCmd::DrawGFP2( 2 );
    }

    // ��ú���
    static void JL_DrawCDH()
    {
        //DrawCmd::DrawCDH();
        DrawCmd::DrawCDH2();
    }

    static void GAS_DisplayDataByDoubleClick()
    {
        DrawCmd::DisplayDataByDoubleClick();
    }

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CDrawGasCmdApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawGOP, DrawGOP, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawGCP, DrawGCP, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawGPP, DrawGPP, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawPPP, DrawPPP, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawCSDCI, DrawCSDCI, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawWDCI1, DrawWDCI1, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawWDCI2, DrawWDCI2, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawWGFP, DrawWGFP, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawTGFP, DrawTGFP, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawCDH, DrawCDH, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, GAS, _DisplayDataByDoubleClick, DisplayDataByDoubleClick, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
