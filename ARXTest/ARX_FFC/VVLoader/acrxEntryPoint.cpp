#include "StdAfx.h"
#include "resource.h"

#include "Utils.h"
#include "SysVarHelper.h"

#include "../MineGEDraw/MineGEDrawSystem.h"

/*
 * �������Է��֣�������޸�/��ԭϵͳ�����Ĵ������kLoadAppMsg��kUnloadAppMsg��
 * ֱ�ӹر�cadʱ������ڴ���ʴ���(���Կ��Կ����������ǵ���״̬cad�ᷢ����������)
 * ����ĸ�Դ��acedSetVar()����������ԭ���в���ȷ
 * ����޸�/��ԭϵͳ�����Ĵ����װ��SysVarHelper���У�������kLoadDwgMsg��kUnloadDwgMsg��
 */

// ����ע���������
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

        // ע�����
        acrxRegisterService( VVLOADER_SERVICE_NAME );

        // ��ʼ�����ӻ�ϵͳ
        if( !initDrawSystem() ) return AcRx::kRetError;

        // ����ͼԪģ��
        if( !loadGEModule() ) return AcRx::kRetError;

        // ��������ģ��
        if( !loadCmdModule() ) return AcRx::kRetError;

        // ����רҵ����ģ��
        if( !loadFunctionModule() ) return AcRx::kRetError;

        acutPrintf( _T( "\nVVLoader::On_kInitAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // ж��רҵ����ģ��
        unloadFunctionModule();

        // ж������ģ��
        unloadCmdModule();

        // ж��ͼԪģ��
        unloadGEModule();

        // �˳����ӻ�ϵͳ
        unInitDrawSystem();

        acutPrintf( _T( "\nVVLoader::On_kUnloadAppMsg\n" ) );

        // ɾ������
        delete acrxServiceDictionary->remove( VVLOADER_SERVICE_NAME );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt );

        // �޸�ȫ������
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

        // ��ʼ��MineGEDrawSystem
        MineGEDrawSystem* pDrawSystem = MineGEDrawSystem::CreateInstance();
        if( pDrawSystem == 0 ) return false;

        pDrawSystem->loadPlugin( BuildPath( GetAppPathDir(), _T( "DefGEPlugin.arx" ) ) );
        pDrawSystem->loadPlugin( BuildPath( GetAppPathDir(), _T( "FFC_GEPlugin.arx" ) ) );

        return true;
    }

    void unInitDrawSystem()
    {
        // �˳�������MineGEServiceϵͳ
        MineGEDrawSystem::DestroyInstance();

        unloadArxModule( _T( "MineGEDraw" ) );
    }

    // ����ͼԪģ��
    bool loadGEModule()
    {
        acutPrintf( _T( "\n-------- ����ͼԪģ�� ------------\n" ) );

        if( !loadArxModule( _T( "MineGE" ) ) ) return false;
        if( !loadArxModule( _T( "DefGE" ) ) ) return false;
        if( !loadArxModule( _T( "FFC_GE" ) ) ) return false;

        return true;
    }

    void unloadGEModule()
    {
        acutPrintf( _T( "\n-------- ж��ͼԪģ�� ------------" ) );

        unloadArxModule( _T( "FFC_GE" ) );
        unloadArxModule( _T( "DefGE" ) );
        unloadArxModule( _T( "MineGE" ) );
    }

    bool loadCmdModule()
    {
        acutPrintf( _T( "\n-------- ��������ģ�� ------------" ) );

        if( !loadArxModule( _T( "MineGECmds" ) ) ) return false;
        if( !loadArxModule( _T( "DrawVentCmd" ) ) ) return false;
        if( !loadArxModule( _T( "DrawFFCCmd" ) ) ) return false;

        return true;
    }

    void unloadCmdModule()
    {
        acutPrintf( _T( "\n-------- ж������ģ�� ------------" ) );

        unloadArxModule( _T( "MineGECmds" ) );
        unloadArxModule( _T( "DrawVentCmd" ) );
        unloadArxModule( _T( "DrawFFCCmd" ) );
    }

    bool loadFunctionModule()
    {
        acutPrintf( _T( "\n-------- ���ع���ģ�� ------------" ) );

        if( !loadArxModule( _T( "ARX_FFC" ) ) ) return false;

        return true;
    }

    void unloadFunctionModule()
    {
        acutPrintf( _T( "\n-------- ж�ع���ģ�� ------------" ) );

        unloadArxModule( _T( "ARX_FFC" ) );
    }

    static void VVTest_yourtest()
    {
    }
} ;

IMPLEMENT_ARX_ENTRYPOINT( CVVLoaderApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CVVLoaderApp, VVTest, _yourtest, yourtest, ACRX_CMD_TRANSPARENT, NULL )
