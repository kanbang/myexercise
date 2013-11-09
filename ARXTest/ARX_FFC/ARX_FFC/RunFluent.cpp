#include "StdAfx.h"

#include "FFC_Def.h"
#include "SysTool.h"
#include "FluentTool.h"
#include "ThreadData.h"

static bool IsFluentExit( FluentThreadData* pData )
{
    DWORD exitCode;
    GetExitCodeProcess( pData->hProcess, &exitCode );

    return ( exitCode != STILL_ACTIVE );
    //return !IsProcessActive(_T("fl6326s.exe"));
}

static bool IsRunError( FluentThreadData* pData )
{
    return IsFileExist( pData->errorFile ) && IsFluentExit( pData );
}

static bool IsRunOK( FluentThreadData* pData )
{
    return IsFileExist( pData->datFile );
}

static bool NeedExitFluent( FluentThreadData* pData )
{
    return ( IsRunError( pData ) || IsRunOK( pData ) );
}

DWORD MyFluentThread( LPVOID lpParam )
{
    // ���߳��м���Ƿ����
    FluentThreadData* pData = ( FluentThreadData* )lpParam;

    while( true )
    {
        Sleep( THREAD_CHECK_TIME_INTERVAL );

        // ��������2������ر��߳�:
        //	1) ������error�ļ������ʾ��������dat�ļ�
        //	2) ����dat�ļ������ʾ����dat�ļ��ɹ�

        if( IsUserAskForThreadEnd() || NeedExitFluent( pData ) )
        {
            CloseHandle( pData->hThread );
            CloseHandle( pData->hProcess );

            // ��ͼ�˳��߳�(û�гɹ�???)
            //DWORD exitCode;
            //GetExitCodeProcess(pData->hProcess, &exitCode);
            //ExitProcess(exitCode);

            break;
        }
    }
    return 0;
}

RUN_ERROR_CODE RunFluent( const CString& appDir, HANDLE& hThread )
{
    CString fluentExePath;
    if( !ReadFluentBinPath( fluentExePath ) )
    {
        return REC_NOT_INSTALL;
    }

    if( !IsFileExist( appDir + FLUENT_LOAD_SCM_FILE ) )
    {
        return REC_FILE_NOT_EXIST;
    }

    // ����fluent
    FluentThreadData data =
    {
        NULL,
        NULL,
        appDir + FLUENT_DAT_FILE,
        appDir + FLUENT_ERROR_FILE
    };

    if( !RunFluent(
                fluentExePath,
                appDir,
                FLUENT_LOAD_SCM_FILE,
                data.hProcess,
                data.hThread,
                true ) )
    {
        return REC_PROCESS_ERROR;
    }

    // ���ݸ�ȫ�ֱ���(����һЩδ֪���̴߳���)
    global_fluent_thread_data = data;

    // ���������߳�
    DWORD dwThreadId;
    /*HANDLE*/
    hThread = CreateThread(
                  NULL,              // default security attributes
                  0,                 // use default stack size
                  ( LPTHREAD_START_ROUTINE )MyFluentThread,        // thread function
                  &global_fluent_thread_data,             // argument to thread function
                  0,                 // use default creation flags
                  &dwThreadId );  // returns the thread identifier

    return REC_NO_EEOR;
}

bool IsRunFluentOk( const CString& appDir )
{
    if( IsFileExist( appDir + FLUENT_ERROR_FILE ) ) return false;
    if( IsFileExist( appDir + FLUENT_DAT_FILE ) ) return true;

    return false;
}