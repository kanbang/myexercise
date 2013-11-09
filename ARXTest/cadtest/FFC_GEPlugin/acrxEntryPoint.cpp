#include "StdAfx.h"
#include "resource.h"

// 定义FFC_GEPlugin ARX模块对应的服务名称
// 用于注册服务和外部加载arx模块的时候
#ifndef FFC_GE_PLUGIN_SERVICE_NAME
#define FFC_GE_PLUGIN_SERVICE_NAME _T("FFC_GEPLUGIN_SERVICE_NAME")
#endif

class CFFC_GEPluginApp : public AcRxArxApp
{

public:
    CFFC_GEPluginApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        acrxRegisterService( FFC_GE_PLUGIN_SERVICE_NAME );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        delete acrxServiceDictionary->remove( FFC_GE_PLUGIN_SERVICE_NAME );

        return ( retCode ) ;
    }

    virtual void RegisterServerComponents ()
    {
    }

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CFFC_GEPluginApp )

