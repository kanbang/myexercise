#include "StdAfx.h"

#include "FFC_Def.h"
#include "SysTool.h"
#include "FluentTool.h"

struct MyFluentThreadData
{
    HANDLE hThread;
    HANDLE hProcess;
    CString datFile;
    CString errorFile;

    bool run_state;  // 运行状态
};

static bool IsRunError( MyFluentThreadData* pData )
{
    DWORD exitCode;
    GetExitCodeProcess( pData->hProcess, &exitCode );

    return IsFileExist( pData->errorFile ) && ( exitCode != STILL_ACTIVE );
}

static bool IsRunOK( MyFluentThreadData* pData )
{
    return IsFileExist( pData->datFile );
}

DWORD MyFluentThread( LPVOID lpParam )
{
    // 在线程中检查是否存在
    MyFluentThreadData* pData = ( MyFluentThreadData* )lpParam;

    while( true )
    {
        Sleep( THREAD_CHECK_TIME_INTERVAL );

        // 根据以下2种情况关闭线程:
        //	1) 发现了error文件，则表示不能生成dat文件
        //	2) 发现dat文件，则表示生成dat文件成功
        if( IsRunError( pData ) || IsRunOK( pData ) )
        {
            // 关闭fluent startup窗口
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

bool RunFluent( const CString& appDir )
{
    CString fluentExePath;
    if( !ReadFluentBinPath( fluentExePath ) )
    {
        AfxMessageBox( _T( "请安装Fluent软件" ) );
        return false;
    }

    // 删除临时文件
    RemoveFile( appDir + FLUENT_CAS_FILE );
    RemoveFile( appDir + FLUENT_DAT_FILE );
    RemoveFile( appDir + FLUENT_ERROR_FILE );

    // 启动fluent
    MyFluentThreadData data = {NULL, NULL, appDir + FLUENT_DAT_FILE, appDir + FLUENT_ERROR_FILE};
    if( !RunFluent(
                fluentExePath,
                appDir,
                FLUENT_LOAD_SCM_FILE,
                data.hProcess,
                data.hThread,
                true ) )
    {
        AfxMessageBox( _T( "启动fluent失败..." ) );
        return false;
    }

    // 创建线程
    DWORD dwThreadId;
    HANDLE hThread = CreateThread(
                         NULL,              // default security attributes
                         0,                 // use default stack size
                         ( LPTHREAD_START_ROUTINE )MyFluentThread,        // thread function
                         &data,             // argument to thread function
                         0,                 // use default creation flags
                         &dwThreadId );  // returns the thread identifier

    WaitForSingleObject( hThread, INFINITE );
    CloseHandle( hThread );

    return true;
}