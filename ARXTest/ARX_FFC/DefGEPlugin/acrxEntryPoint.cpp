#include "StdAfx.h"
#include "resource.h"

// ����DefGEPlugin ARXģ���Ӧ�ķ�������
// ����ע�������ⲿ����arxģ���ʱ��
#ifndef DEFGE_PLUGIN_SERVICE_NAME
#define DEFGE_PLUGIN_SERVICE_NAME _T("DEFGEPLUGIN_SERVICE_NAME")
#endif

class CDefGEPluginApp : public AcRxArxApp
{

public:
    CDefGEPluginApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        acrxRegisterService( DEFGE_PLUGIN_SERVICE_NAME );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        delete acrxServiceDictionary->remove( DEFGE_PLUGIN_SERVICE_NAME );

        return ( retCode ) ;
    }

    virtual void RegisterServerComponents ()
    {
    }

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CDefGEPluginApp )

