#include "StdAfx.h"
#include "resource.h"

// ����FFC_GEPlugin ARXģ���Ӧ�ķ�������
// ����ע�������ⲿ����arxģ���ʱ��
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

