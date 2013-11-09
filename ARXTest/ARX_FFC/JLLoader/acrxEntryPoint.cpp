#include "StdAfx.h"
#include "resource.h"

#include "Utils.h"
#include "UIHelper.h"

#ifndef JLLOADER_SERVICE_NAME
#define JLLOADER_SERVICE_NAME _T("JLLOADER_SERVICE_NAME")
#endif

class CJLLoaderApp : public AcRxArxApp
{
public:
    CJLLoaderApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // ע�����
        acrxRegisterService( JLLOADER_SERVICE_NAME );

        // ��Ӳ˵�
        //UIHelper::RegAppMenu();

        // ����CUI�ļ�
        if( !LoadAppAndUI() ) return AcRx::kRetError;

        // ���֧��·��
        //AddSupportPath(GetAppPathDir());

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg( pkt );

        // ж�ز˵�
        //UIHelper::UnRegAppMenu();

        // ��������Ѿ�����, ж��ԭ�еĳ���
        UnLoadAppAndUI();

        // ɾ�����֧��·��
        //RemoveSupportPath(GetAppPathDir());

        // ɾ������
        delete acrxServiceDictionary->remove( JLLOADER_SERVICE_NAME );

        return ( retCode ) ;
    }

    virtual void RegisterServerComponents()
    {
    }

    static bool LoadAppAndUI()
    {
        if( !loadArxModule( _T( "VVLoader" ) ) ) return false;
        if( !loadArxModule( _T( "ARX_FFC_UI" ) ) ) return false;

        return true;
    }

    static void UnLoadAppAndUI()
    {
        unloadArxModule( _T( "ARX_FFC_UI" ) );
        unloadArxModule( _T( "VVLoader" ) );
    }

    static void JL_LoadARXProgram()
    {
        if( !loadArxModule( _T( "VVLoader" ) ) ) return /*AcRx::kRetError*/;
        if( !loadArxModule( _T( "ARX_FFC_UI" ) ) ) return /*AcRx::kRetError*/;
    }

    static void JL_UnLoadARXProgram()
    {
        unloadArxModule( _T( "ARX_FFC_UI" ) );
        unloadArxModule( _T( "VVLoader" ) );
    }
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CJLLoaderApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CJLLoaderApp, JL, _LoadARXProgram, LoadARXProgram, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CJLLoaderApp, JL, _UnLoadARXProgram, UnLoadARXProgram, ACRX_CMD_TRANSPARENT, NULL )
