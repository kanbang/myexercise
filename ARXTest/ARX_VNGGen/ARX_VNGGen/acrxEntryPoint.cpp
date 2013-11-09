#include "StdAfx.h"

#include "UIHelper.h"
#include "DrawCmd.h"
#include "VNG_ParamHelper.h"

// 定义注册服务名称
#ifndef ARX_VNGGEN_SERVICE_NAME
#define ARX_VNGGEN_SERVICE_NAME _T("ARX_VNGGEN_SERVICE_NAME")
#endif

class CARX_VNGGenApp : public AcRxArxApp
{

public:
    CARX_VNGGenApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // 注册服务
        acrxRegisterService( ARX_VNGGEN_SERVICE_NAME );

        //acutPrintf(_T("\nARX_VNGGen::On_kInitAppMsg\n"));

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // 删除服务
        delete acrxServiceDictionary->remove( ARX_VNGGEN_SERVICE_NAME );

        UIHelper::DestroyDataListDockBar();

        //acutPrintf(_T("\nARX_VNGGen::On_kUnloadAppMsg\n"));

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt ) ;

        // 初始化绘制参数
        VNG_ParamHelper::InitParams();
        //acutPrintf(_T("\nARX_VNGGen::On_kLoadDwgMsg\n"));

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg( pkt ) ;

        //acutPrintf(_T("\nARX_VNGGen::On_kUnloadDwgMsg\n"));

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    static void JL_DrawVentNetWorkGraph( void )
    {
        UIHelper::ShowVNGGenDlg();
    }

    static void JL_ShowDataList( void )
    {
        UIHelper::ShowDataListDockBar();
    }

    static void JL_DisplayDataByDoubleClick()
    {
        UIHelper::ShowPropertyDlgByDoubleClick();
    }

    static void JL_SetNodeParam()
    {
        UIHelper::SetNodeParam();
    }

    static void JL_SetEdgeParam()
    {
        UIHelper::SetEdgeParam();
    }

    static void JL_SetGateParam()
    {
        UIHelper::SetGateParam();
    }

    static void JL_SetFanParam()
    {
        UIHelper::SetFanParam();
    }

    static void JL_ResetGate()
    {
        DrawCmd::ResetGate();
    }

    static void JL_ResetFan()
    {
        DrawCmd::ResetFan();
    }
} ;

IMPLEMENT_ARX_ENTRYPOINT( CARX_VNGGenApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CARX_VNGGenApp, JL, _DrawVentNetWorkGraph, DrawVentNetWorkGraph, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CARX_VNGGenApp, JL, _ShowDataList, ShowDataList, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CARX_VNGGenApp, JL, _DisplayDataByDoubleClick, DisplayDataByDoubleClick, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CARX_VNGGenApp, JL, _SetNodeParam, SetNodeParam, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CARX_VNGGenApp, JL, _SetEdgeParam, SetEdgeParam, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CARX_VNGGenApp, JL, _SetGateParam, SetGateParam, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CARX_VNGGenApp, JL, _SetFanParam, SetFanParam, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CARX_VNGGenApp, JL, _ResetGate, ResetGate, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CARX_VNGGenApp, JL, _ResetFan, ResetFan, ACRX_CMD_TRANSPARENT, NULL )
