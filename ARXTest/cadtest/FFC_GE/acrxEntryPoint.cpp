#include "StdAfx.h"
#include "resource.h"

// ����DefGE ARXģ���Ӧ�ķ�������
// ����ע�������ⲿ����arxģ���ʱ��
#ifndef FFC_GE_SERVICE_NAME
#define FFC_GE_SERVICE_NAME _T("FFC_GE_SERVICE_NAME")
#endif

class CFFC_GEApp : public AcRxArxApp
{

public:
    CFFC_GEApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // ע�����
        acrxRegisterService( FFC_GE_SERVICE_NAME );

        acutPrintf( _T( "\nFFC_GE::On_kLoadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // ɾ������
        delete acrxServiceDictionary->remove( FFC_GE_SERVICE_NAME );

        acutPrintf( _T( "\nFFC_GE::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt ) ;

        acutPrintf( _T( "\nFFC_GE::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg ( pkt ) ;

        acutPrintf( _T( "\nFFC_GE::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CFFC_GEApp )

