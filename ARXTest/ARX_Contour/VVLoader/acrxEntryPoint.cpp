#include "StdAfx.h"
#include "resource.h"

#include "Utils.h"
#include "SysVarHelper.h"

#include "../MineGEDraw/MineGEDrawSystem.h"

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

        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

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

        delete acrxServiceDictionary->remove( VVLOADER_SERVICE_NAME );

        acutPrintf( _T( "\nVVLoader::On_kUnloadAppMsg\n" ) );

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

    bool loadArxModule( const CString& arxModuleName )
    {
        CString arxName = BuildArxFileName( arxModuleName );
        CString serviceName = BuildServiceName( arxModuleName );

        AcRxObject* pSvc = acrxServiceDictionary->at ( serviceName );
        if( pSvc == NULL )
        {
            if( !acrxDynamicLinker->loadModule( arxName, false ) )
            {
                acutPrintf ( _T( "\n����%sģ��ʧ��...\n" ), arxName ) ;
                return false;
            }
            acutPrintf ( _T( "\n�ɹ�����%sģ��...\n" ), arxName ) ;
        }
        else
        {
            acutPrintf ( _T( "\n%sģ���Ѿ����ع���...\n" ), arxName ) ;
        }
        return true;
    }

    void unloadArxModule( const CString& arxModuleName )
    {
        acrxDynamicLinker->unloadModule( BuildArxFileName( arxModuleName ), 0 );
        acutPrintf( _T( "\nж��ģ��:%s\n" ), arxModuleName );
    }

    bool initDrawSystem()
    {
        if( !loadArxModule( _T( "MineGEDraw" ) ) ) return false;

        // ��ʼ��MineGEDrawSystem
        MineGEDrawSystem::CreateInstance();
        MineGEDrawSystem* pDrawSystem = MineGEDrawSystem::GetInstance();

        pDrawSystem->loadPlugin( BuildPath( GetAppPathDir(), _T( "GasGEPlugin.arx" ) ) );

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
        if( !loadArxModule( _T( "GasGE" ) ) ) return false;
        if( !loadArxModule( _T( "ContourGE" ) ) ) return false;

        return true;
    }

    void unloadGEModule()
    {
        acutPrintf( _T( "\n-------- ж��ͼԪģ�� ------------" ) );

        unloadArxModule( _T( "ContourGE" ) );
        unloadArxModule( _T( "GasGE" ) );
        unloadArxModule( _T( "MineGE" ) );
    }

    bool loadCmdModule()
    {
        acutPrintf( _T( "\n-------- ��������ģ�� ------------" ) );

        if( !loadArxModule( _T( "DrawGasCmd" ) ) ) return false;

        return true;
    }

    void unloadCmdModule()
    {
        acutPrintf( _T( "\n-------- ж������ģ�� ------------" ) );

        unloadArxModule( _T( "DrawGasCmd" ) );
    }

    bool loadFunctionModule()
    {
        acutPrintf( _T( "\n-------- ���ع���ģ�� ------------" ) );

        if( !loadArxModule( _T( "ARX_Contour" ) ) ) return false;

        return true;
    }

    void unloadFunctionModule()
    {
        acutPrintf( _T( "\n-------- ж�ع���ģ�� ------------" ) );

        unloadArxModule( _T( "ARX_Contour" ) );
    }

    static void VVTest_testVector()
    {

    }
} ;

IMPLEMENT_ARX_ENTRYPOINT( CVVLoaderApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CVVLoaderApp, VVTest, _testVector, tvec, ACRX_CMD_TRANSPARENT, NULL )