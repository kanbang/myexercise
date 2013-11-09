#include "StdAfx.h"
#include "resource.h"

#include "ContourCmd.h"
#include "ContourParamHelper.h"

#include "ContourSelectFilter.h"
ContourSelectFilter* pGlobalOnlyOneSelectFilter;

// 定义注册服务名称
#ifndef ARX_CONTOUR_SERVICE_NAME
#define ARX_CONTOUR_SERVICE_NAME _T("ARX_CONTOUR_SERVICE_NAME")
#endif

class CContourApp : public AcRxArxApp
{

public:
    CContourApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // 注册服务
        acrxRegisterService( ARX_CONTOUR_SERVICE_NAME );

        acutPrintf( _T( "\nContour::On_kLoadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // 删除服务
        delete acrxServiceDictionary->remove( ARX_CONTOUR_SERVICE_NAME );

        acutPrintf( _T( "\nARX_Contour::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt ) ;

        /* 全局函数(RegFields.cpp) */
        extern void RegFields();

        // 注册字段
        RegFields();

        // 初始化等值线的一些参数
        ContourParamHelper::InitParams();

        pGlobalOnlyOneSelectFilter = new ContourSelectFilter( curDoc() );

        acutPrintf( _T( "\nARX_Contour::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg ( pkt ) ;

        delete pGlobalOnlyOneSelectFilter;
        pGlobalOnlyOneSelectFilter = 0;

        acutPrintf( _T( "\nContour::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    static void JL_DrawGasContour( void )
    {
        ContourCmd::ShowContourDlg();
    }

    static void JL_ShowContourParamDlg( void )
    {
        ContourCmd::ShowContourParamDlg();
    }

    static void JL_ShowContourLayerDlg()
    {
        ContourCmd::ShowContourLayerDlg();
    }

    static void JL_ShowContourColorDlg()
    {
        ContourCmd::ShowContourColorDlg();
    }

    static void JL_ShowPointForecastDlg( void )
    {
        ContourCmd::ShowPointForecastDlg();
    }

    static void JL_ContourTest( void )
    {
        extern void ContourTest();

        ContourTest();
    }

    static void JL_TestData( void )
    {
        extern void writeDataToFile();
        writeDataToFile();
    }

    static void JL_ShowRegionForecastDlg( void )
    {
        ContourCmd::ShowRegionForecastDlg();
    }

    static void JL_PolyLineJigTest( void )
    {
        ContourCmd::PolyLineJigTest();
    }
} ;

IMPLEMENT_ARX_ENTRYPOINT( CContourApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CContourApp, JL, _DrawGasContour, DrawGasContour, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CContourApp, JL, _ShowContourParamDlg, ShowContourParamDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CContourApp, JL, _ShowContourLayerDlg, ShowContourLayerDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CContourApp, JL, _ShowContourColorDlg, ShowContourColorDlg, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CContourApp, JL, _ShowPointForecastDlg, ShowPointForecastDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CContourApp, JL, _ShowRegionForecastDlg, ShowRegionForecastDlg, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CContourApp, JL, _ContourTest, ContourTest, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CContourApp, JL, _TestData, TestData, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CContourApp, JL, _PolyLineJigTest, PolyLineJigTest, ACRX_CMD_TRANSPARENT, NULL )
