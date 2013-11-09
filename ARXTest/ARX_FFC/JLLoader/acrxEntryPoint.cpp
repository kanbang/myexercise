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

        // 注册服务
        acrxRegisterService( JLLOADER_SERVICE_NAME );

        // 添加菜单
        //UIHelper::RegAppMenu();

        // 加载CUI文件
        if( !LoadAppAndUI() ) return AcRx::kRetError;

        // 添加支持路径
        //AddSupportPath(GetAppPathDir());

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg( pkt );

        // 卸载菜单
        //UIHelper::UnRegAppMenu();

        // 如果程序已经加载, 卸载原有的程序
        UnLoadAppAndUI();

        // 删除添加支持路径
        //RemoveSupportPath(GetAppPathDir());

        // 删除服务
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
