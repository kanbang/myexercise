#include "StdAfx.h"

#include "FFC_Def.h"
#include "SysTool.h"
#include "FluentTool.h"


struct MyGambitThreadData
{
    HANDLE hThread;
    HANDLE hProcess;
    CString mshFile;
    CString errorFile;
};

static bool IsRunError( MyGambitThreadData* pData )
{
    return IsFileExist( pData->errorFile );
}

static bool IsRunOK( MyGambitThreadData* pData )
{
    return IsFileExist( pData->mshFile );
}

DWORD MyGambitThread( LPVOID lpParam )
{
    MyGambitThreadData* pData = ( MyGambitThreadData* )lpParam;

    while( true )
    {
        Sleep( THREAD_CHECK_TIME_INTERVAL );

        // 根据以下2种情况关闭线程:
        //	1) 发现了error文件，则表示不能生成msh文件
        //	2) 发现msh文件，则表示生成msh文件成功
        if( IsRunError( pData ) || IsRunOK( pData ) )
        {
            // 关闭gambit startup窗口
            CloseGambitStartupWindow();
            CloseHandle( pData->hThread );
            CloseHandle( pData->hProcess );

            // 试图退出线程(没有成功???)
            //DWORD exitCode;
            //GetExitCodeProcess(pData->hProcess, &exitCode);
            //ExitProcess(exitCode);

            break;
        }
    }
    return 0;
}

bool RunGambit( const CString& appDir )
{
    CString gambitExePath;
    if( !ReadGambitBinPath( gambitExePath ) )
    {
        AfxMessageBox( _T( "请安装Gambit软件" ) );
        return false;
    }

    RemoveFile( appDir + GAMBIT_JOU_FILE );
    RemoveFile( appDir + GAMBIT_TRN_FILE );
    RemoveFile( appDir + GAMBIT_DBS_FILE );
    RemoveFile( appDir + GAMBIT_ERROR_FILE );

    // 启动gambit
    MyGambitThreadData data = {NULL, NULL, appDir + GAMBIT_MESH_FILE, appDir + GAMBIT_ERROR_FILE};
    if( !RunGambit(
                gambitExePath,
                appDir,
                GAMBIT_VAR_JOU_FIEL_NAME,
                data.hProcess,
                data.hThread,
                true ) )
    {
        AfxMessageBox( _T( "启动gambit失败..." ) );
        return false;
    }

    // 创建线程
    DWORD dwThreadId;
    HANDLE hThread = CreateThread(
                         NULL,              // default security attributes
                         0,                 // use default stack size
                         ( LPTHREAD_START_ROUTINE )MyGambitThread,        // thread function
                         &data,             // argument to thread function
                         0,                 // use default creation flags
                         &dwThreadId );  // returns the thread identifier

    WaitForSingleObject( hThread, INFINITE );
    CloseHandle( hThread );

    return true;
}