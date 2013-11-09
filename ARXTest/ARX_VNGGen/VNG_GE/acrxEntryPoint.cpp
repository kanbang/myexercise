#include "StdAfx.h"
#include "resource.h"

// ����ע���������
#ifndef VNG_GE_SERVICE_NAME
#define VNG_GE_SERVICE_NAME _T("VNG_GE_SERVICE_NAME")
#endif

class CVNG_GEApp : public AcRxArxApp
{

public:
    CVNG_GEApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // ע�����
        acrxRegisterService( VNG_GE_SERVICE_NAME );

        //acutPrintf(_T("\nVNG_GE::On_kInitAppMsg\n"));

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // ɾ������
        delete acrxServiceDictionary->remove( VNG_GE_SERVICE_NAME );

        //acutPrintf(_T("\nVNG_GE::On_kUnloadAppMsg\n"));

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt ) ;

        //acutPrintf(_T("\nVNG_GE::On_kLoadDwgMsg\n"));

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg( pkt ) ;

        //acutPrintf(_T("\nVNG_GE::On_kUnloadDwgMsg\n"));

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

} ;

IMPLEMENT_ARX_ENTRYPOINT( CVNG_GEApp )

