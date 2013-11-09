#include "StdAfx.h"
#include "resource.h"

#define szRDS _RXST("")

// 定义服务名称
// 格式: ${输出arx模块名称大写}_SERVICE_NAME
// 例如arx输出模块名称：EvalBase.arx
// 那么服务名称:EVALBASE_SERVICE_NAME
#ifndef EVAL_BASE_SERVICE_NAME
#define EVAL_BASE_SERVICE_NAME _T("EVALBASE_SERVICE_NAME")
#endif

class CEvalBaseApp : public AcRxArxApp
{

public:
    CEvalBaseApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        acrxRegisterService( EVAL_BASE_SERVICE_NAME );

        acutPrintf( _T( "\nEvalBase::On_kInitAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        delete acrxServiceDictionary->remove( EVAL_BASE_SERVICE_NAME );

        acutPrintf( _T( "\nEvalBase::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt );

        acutPrintf( _T( "\nEvalBase::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg( pkt ) ;

        acutPrintf( _T( "\nEvalBase::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CEvalBaseApp )

