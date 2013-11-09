#include "StdAfx.h"
#include "resource.h"

// 定义注册服务名称
#ifndef VENTNETWORK_ALGO_SERVICE_NAME
#define VENTNETWORK_ALGO_SERVICE_NAME _T("VENTNETWORKALGO_SERVICE_NAME")
#endif

#include "VNAlgoTest.h"
#include "VNAlgoHelper.h"

class CVentNetworkAlgoApp : public AcRxArxApp
{

public:
    CVentNetworkAlgoApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // 注册服务
        acrxRegisterService( VENTNETWORK_ALGO_SERVICE_NAME );

        acutPrintf( _T( "\nVentNetworkAlgo::On_kInitAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // 删除服务
        delete acrxServiceDictionary->remove( VENTNETWORK_ALGO_SERVICE_NAME );

        acutPrintf( _T( "\nVentNetworkAlgo::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt ) ;

        acutPrintf( _T( "\nVentNetworkAlgo::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg ( pkt ) ;

        acutPrintf( _T( "\nVentNetworkAlgo::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    static void VentNetworkAlgo_FIPTest( void )
    {
        acutPrintf( _T( "\nfip测试..." ) );
        VNAlgoTest::FIPTest();
    }

    static void VentNetworkAlgo_SWDTest( void )
    {
        acutPrintf( _T( "\nswd测试..." ) );
        VNAlgoTest::SWDTest();
    }

    static void VentNetworkAlgo_ILTTest2( void )
    {
        acutPrintf( _T( "\nILT测试2..." ) );
        VNAlgoTest::ILTTest2();
    }

    static void VentNetworkAlgo_ILTTest3( void )
    {
        acutPrintf( _T( "\nILT测试3..." ) );
        VNAlgoTest::ILTTest3();
    }

    static void VentNetworkAlgo_DFSTest( void )
    {
        acutPrintf( _T( "\nDFS测试..." ) );
        VNAlgoTest::DFSTest();
    }

    static void VentNetworkAlgo_IntFlowTest( void )
    {
        acutPrintf( _T( "\nIntFlow测试..." ) );
        VNAlgoTest::IntFlowTest();
    }

    static void VentNetworkAlgo_FWSTest( void )
    {
        acutPrintf( _T( "\nFWSTest测试..." ) );
        VNAlgoTest::FWSTest();
    }

    static void VentNetworkAlgo_IsIDSTest( void )
    {
        acutPrintf( _T( "\n判断是否角联分支..." ) );
        VNAlgoTest::IsIDSTest();
    }

    static void VentNetworkAlgo_DoIDSTest( void )
    {
        acutPrintf( _T( "\n查找分支的角联结构..." ) );
        VNAlgoTest::DoIDSTest( false );
    }

    static void VentNetworkAlgo_DoIDSTest2( void )
    {
        acutPrintf( _T( "\n查找分支的角联结构2..." ) );
        VNAlgoTest::DoIDSTest( true );
    }
} ;

IMPLEMENT_ARX_ENTRYPOINT( CVentNetworkAlgoApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CVentNetworkAlgoApp, VentNetworkAlgo, _FIPTest, FIPTest, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CVentNetworkAlgoApp, VentNetworkAlgo, _SWDTest, SWDTest, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CVentNetworkAlgoApp, VentNetworkAlgo, _ILTTest2, ILTTest2, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CVentNetworkAlgoApp, VentNetworkAlgo, _ILTTest3, ILTTest3, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CVentNetworkAlgoApp, VentNetworkAlgo, _FWSTest, FWSTest, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CVentNetworkAlgoApp, VentNetworkAlgo, _IntFlowTest, IntFlowTest, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CVentNetworkAlgoApp, VentNetworkAlgo, _DFSTest, DFSTest, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CVentNetworkAlgoApp, VentNetworkAlgo, _IsIDSTest, IsIDSTest, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CVentNetworkAlgoApp, VentNetworkAlgo, _DoIDSTest, DoIDSTest, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CVentNetworkAlgoApp, VentNetworkAlgo, _DoIDSTest2, DoIDSTest2, ACRX_CMD_TRANSPARENT, NULL )
