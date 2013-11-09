#include "StdAfx.h"
#include "resource.h"

// ����GasGEPlugin ARXģ���Ӧ�ķ�������
// ����ע�������ⲿ����arxģ���ʱ��
#ifndef GASGE_PLUGIN_SERVICE_NAME
#define GASGE_PLUGIN_SERVICE_NAME _T("GASGEPLUGIN_SERVICE_NAME")
#endif

class CGasGEPluginApp : public AcRxArxApp
{

public:
    CGasGEPluginApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        acrxRegisterService( GASGE_PLUGIN_SERVICE_NAME );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        delete acrxServiceDictionary->remove( GASGE_PLUGIN_SERVICE_NAME );

        return ( retCode ) ;
    }

    virtual void RegisterServerComponents ()
    {
    }

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CGasGEPluginApp )

