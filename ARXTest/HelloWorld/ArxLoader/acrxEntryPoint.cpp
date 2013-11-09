#include "StdAfx.h"
#include "resource.h"

// 定义注册服务名称
#ifndef ARX_LOADER_SERVICE_NAME
#define ARX_LOADER_SERVICE_NAME _T("ARX_LOADER_SERVICE_NAME")
#endif

#include "Utils.h"
#include "ReactorHelper.h"

class CArxLoaderApp : public AcRxArxApp
{

public:
    CArxLoaderApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt );

        // 注册服务
        acrxRegisterService( ARX_LOADER_SERVICE_NAME );

        CString cuiFile = _T( "\"C:\\Users\\anheihb03dlj\\Desktop\\铁煤大兴矿采空区风火瓦斯耦合模拟\\ui\\铁煤大兴矿采空区风火瓦斯耦合模拟.cuix\"" );
        //CString arxApp = _T("ARX_FFC");
        //CString arxFile = appDir + _T("\\bin\\") + _T("VVLoader.arx");
        //CString cuiFile = appDir + _T("\\ui\\") + _T("铁煤大兴矿采空区风火瓦斯耦合模拟.cuix");
        CString mainCui = _T( "\"C:\\Users\\anheihb03dlj\\appdata\\roaming\\autodesk\\autocad 2010\\r18.0\\chs\\support\\acad.cuix\"" );

        //AddSupportPath(appDir+"\\bin\\");

        //ReactorHelper::CreateCadQuitEditorReactor();
        //ReactorHelper::CreateDocReactor();
        //ReactorHelper::CreateDBReactor();

//		UnLoadPartialMenuUseCom(CUI_FILE_GROUP_NAME);
        //UnLoadPartialMenuUseCmd(CUI_FILE_GROUP_NAME);
        //UnLoadPartialMenuUseCmd(_T("ACAD"));
        //LoadPartialMenuUseCmd(mainCui, _T("ACAD"));
        //LoadPartialMenuUseCmd(cuiFile, CUI_FILE_GROUP_NAME);


        //AddLoaderMenu();

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // 删除服务
        delete acrxServiceDictionary->remove( ARX_LOADER_SERVICE_NAME );

        CString cuiFile = _T( "\"C:\\Users\\anheihb03dlj\\Desktop\\铁煤大兴矿采空区风火瓦斯耦合模拟\\ui\\铁煤大兴矿采空区风火瓦斯耦合模拟.cuix\"" );
        //CString arxApp = _T("ARX_FFC");
        //CString arxFile = appDir + _T("\\bin\\") + _T("VVLoader.arx");
        //CString cuiFile = appDir + _T("\\ui\\") + _T("铁煤大兴矿采空区风火瓦斯耦合模拟.cuix");
        CString mainCui = _T( "\"C:\\Users\\anheihb03dlj\\appdata\\roaming\\autodesk\\autocad 2010\\r18.0\\chs\\support\\acad.cuix\"" );

        //ReactorHelper::RemoveCadQuitEditorReactor();
        //ReactorHelper::RemoveDocReactor();
        //ReactorHelper::RemoveDBReactor();
        //if(!acDocManager->isApplicationContext())
        //{
        //	//UnLoadPartialMenuUseCom(CUI_FILE_GROUP_NAME);
        //	UnLoadPartialMenuUseCmd(CUI_FILE_GROUP_NAME);
        //}

        //RemoveLoaderMenu();

        return ( retCode ) ;
    }

    virtual void RegisterServerComponents()
    {
    }

    static void JL_LoadArxProgram()
    {
        CString cuiFile = _T( "\"C:\\Users\\anheihb03dlj\\Desktop\\铁煤大兴矿采空区风火瓦斯耦合模拟\\ui\\铁煤大兴矿采空区风火瓦斯耦合模拟.cuix\"" );
        //CString arxApp = _T("ARX_FFC");
        //CString arxFile = appDir + _T("\\bin\\") + _T("VVLoader.arx");
        //CString cuiFile = appDir + _T("\\ui\\") + _T("铁煤大兴矿采空区风火瓦斯耦合模拟.cuix");
        CString mainCui = _T( "\"C:\\Users\\anheihb03dlj\\appdata\\roaming\\autodesk\\autocad 2010\\r18.0\\chs\\support\\acad.cuix\"" );

        //CMDIFrameWnd * pMainFrame = acedGetAcadFrame();
        //pMainFrame->LockWindowUpdate();

        LoadPartialMenuUseCom( cuiFile, CUI_FILE_GROUP_NAME );
        //LoadPartialMenuUseCmd(cuiFile, CUI_FILE_GROUP_NAME);

        //pMainFrame->UnlockWindowUpdate();
    }

    static void JL_UnLoadArxProgram()
    {
        CString cuiFile = _T( "\"C:\\Users\\anheihb03dlj\\Desktop\\铁煤大兴矿采空区风火瓦斯耦合模拟\\ui\\铁煤大兴矿采空区风火瓦斯耦合模拟.cuix\"" );
        //CString arxApp = _T("ARX_FFC");
        //CString arxFile = appDir + _T("\\bin\\") + _T("VVLoader.arx");
        //CString cuiFile = appDir + _T("\\ui\\") + _T("铁煤大兴矿采空区风火瓦斯耦合模拟.cuix");
        CString mainCui = _T( "\"C:\\Users\\anheihb03dlj\\appdata\\roaming\\autodesk\\autocad 2010\\r18.0\\chs\\support\\acad.cuix\"" );
        //CMDIFrameWnd * pMainFrame = acedGetAcadFrame();
        //pMainFrame->LockWindowUpdate();

        //UnLoadPartialMenuUseCom(CUI_FILE_GROUP_NAME);
        UnLoadPartialMenuUseCmd( CUI_FILE_GROUP_NAME );
        //UnLoadPartialMenuUseCmd(_T("ACAD"));

        //pMainFrame->UnlockWindowUpdate();
    }
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CArxLoaderApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CArxLoaderApp, JL, _LoadArxProgram, LoadArxProgram, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CArxLoaderApp, JL, _UnLoadArxProgram, UnLoadArxProgram, ACRX_CMD_TRANSPARENT, NULL )