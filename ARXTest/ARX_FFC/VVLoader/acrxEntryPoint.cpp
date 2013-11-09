#include "StdAfx.h"
#include "resource.h"

#include "Utils.h"
#include "SysVarHelper.h"

#include "../MineGEDraw/MineGEDrawSystem.h"

/*
 * 经过测试发现，如果将修改/还原系统变量的代码放在kLoadAppMsg和kUnloadAppMsg中
 * 直接关闭cad时会出现内存访问错误(调试可以看出来，而非调试状态cad会发出错误声音)
 * 问题的根源在acedSetVar()函数，具体原因尚不明确
 * 最后将修改/还原系统变量的代码封装到SysVarHelper类中，并放在kLoadDwgMsg和kUnloadDwgMsg中
 */

// 定义注册服务名称
#ifndef VVLOADER_SERVICE_NAME
#define VVLOADER_SERVICE_NAME _T("VVLOADER_SERVICE_NAME")
#endif

class CVVLoaderApp : public AcRxArxApp
{

public:
    CVVLoaderApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt );

        // 注册服务
        acrxRegisterService( VVLOADER_SERVICE_NAME );

        // 初始化可视化系统
        if( !initDrawSystem() ) return AcRx::kRetError;

        // 加载图元模块
        if( !loadGEModule() ) return AcRx::kRetError;

        // 加载命令模块
        if( !loadCmdModule() ) return AcRx::kRetError;

        // 加载专业功能模块
        if( !loadFunctionModule() ) return AcRx::kRetError;

        acutPrintf( _T( "\nVVLoader::On_kInitAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // 卸载专业功能模块
        unloadFunctionModule();

        // 卸载命令模块
        unloadCmdModule();

        // 卸载图元模块
        unloadGEModule();

        // 退出可视化系统
        unInitDrawSystem();

        acutPrintf( _T( "\nVVLoader::On_kUnloadAppMsg\n" ) );

        // 删除服务
        delete acrxServiceDictionary->remove( VVLOADER_SERVICE_NAME );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt );

        // 修改全局数据
        SysVarHelper::ModifyTolerance();
        SysVarHelper::ModifySelectPreview();
        SysVarHelper::ModifyPickAdd();

        acutPrintf( _T( "\nVVLoader::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg( pkt ) ;

        SysVarHelper::RestoreTolerace();
        SysVarHelper::RestoreSelectPreview();
        SysVarHelper::RestorePickAdd();

        acutPrintf( _T( "\nVVLoader::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    bool initDrawSystem()
    {
        if( !loadArxModule( _T( "MineGEDraw" ) ) ) return false;

        // 初始化MineGEDrawSystem
        MineGEDrawSystem* pDrawSystem = MineGEDrawSystem::CreateInstance();
        if( pDrawSystem == 0 ) return false;

        pDrawSystem->loadPlugin( BuildPath( GetAppPathDir(), _T( "DefGEPlugin.arx" ) ) );
        pDrawSystem->loadPlugin( BuildPath( GetAppPathDir(), _T( "FFC_GEPlugin.arx" ) ) );

        return true;
    }

    void unInitDrawSystem()
    {
        // 退出并清理MineGEService系统
        MineGEDrawSystem::DestroyInstance();

        unloadArxModule( _T( "MineGEDraw" ) );
    }

    // 加载图元模块
    bool loadGEModule()
    {
        acutPrintf( _T( "\n-------- 加载图元模块 ------------\n" ) );

        if( !loadArxModule( _T( "MineGE" ) ) ) return false;
        if( !loadArxModule( _T( "DefGE" ) ) ) return false;
        if( !loadArxModule( _T( "FFC_GE" ) ) ) return false;

        return true;
    }

    void unloadGEModule()
    {
        acutPrintf( _T( "\n-------- 卸载图元模块 ------------" ) );

        unloadArxModule( _T( "FFC_GE" ) );
        unloadArxModule( _T( "DefGE" ) );
        unloadArxModule( _T( "MineGE" ) );
    }

    bool loadCmdModule()
    {
        acutPrintf( _T( "\n-------- 加载命令模块 ------------" ) );

        if( !loadArxModule( _T( "MineGECmds" ) ) ) return false;
        if( !loadArxModule( _T( "DrawVentCmd" ) ) ) return false;
        if( !loadArxModule( _T( "DrawFFCCmd" ) ) ) return false;

        return true;
    }

    void unloadCmdModule()
    {
        acutPrintf( _T( "\n-------- 卸载命令模块 ------------" ) );

        unloadArxModule( _T( "MineGECmds" ) );
        unloadArxModule( _T( "DrawVentCmd" ) );
        unloadArxModule( _T( "DrawFFCCmd" ) );
    }

    bool loadFunctionModule()
    {
        acutPrintf( _T( "\n-------- 加载功能模块 ------------" ) );

        if( !loadArxModule( _T( "ARX_FFC" ) ) ) return false;

        return true;
    }

    void unloadFunctionModule()
    {
        acutPrintf( _T( "\n-------- 卸载功能模块 ------------" ) );

        unloadArxModule( _T( "ARX_FFC" ) );
    }

    static void VVTest_yourtest()
    {
    }
} ;

IMPLEMENT_ARX_ENTRYPOINT( CVVLoaderApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CVVLoaderApp, VVTest, _yourtest, yourtest, ACRX_CMD_TRANSPARENT, NULL )
