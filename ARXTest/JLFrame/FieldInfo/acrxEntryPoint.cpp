#include "StdAfx.h"
#include "resource.h"

#include "config.h"
#include "../ArxHelper/HelperClass.h"

#ifndef FIELDINFO_SERVICE_NAME
#define FIELDINFO_SERVICE_NAME _T("FIELDINFO_SERVICE_NAME")
#endif

class CFieldInfoApp : public AcRxArxApp
{

public:
    CFieldInfoApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        acrxRegisterService( FIELDINFO_SERVICE_NAME );

        acutPrintf( _T( "\nFieldInfo::On_kLoadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        delete acrxServiceDictionary->remove( FIELDINFO_SERVICE_NAME );

        acutPrintf( _T( "\nFieldInfo::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt ) ;

        // ×¢²á´Êµä
        ArxDataTool::RegDict( PROPERTY_DATA_FIELD_INFO_DICT );

        acutPrintf( _T( "\nFieldInfo::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg ( pkt ) ;

        acutPrintf( _T( "\nFieldInfo::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CFieldInfoApp )

