#include "StdAfx.h"
#include "resource.h"

class CVNGGEPluginApp : public AcRxArxApp
{

public:
    CVNGGEPluginApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        return ( retCode ) ;
    }

    virtual void RegisterServerComponents ()
    {
    }

} ;

IMPLEMENT_ARX_ENTRYPOINT( CVNGGEPluginApp )
