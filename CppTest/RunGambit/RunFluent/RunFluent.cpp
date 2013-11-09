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

    bool run_state;  // ����״̬
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
    // ���߳��м���Ƿ����
    MyFluentThreadData* pData = ( MyFluentThreadData* )lpParam;

    while( true )
    {
        Sleep( THREAD_CHECK_TIME_INTERVAL );

        // ��������2������ر��߳�:
        //	1) ������error�ļ������ʾ��������dat�ļ�
        //	2) ����dat�ļ������ʾ����dat�ļ��ɹ�
        if( IsRunError( pData ) || IsRunOK( pData ) )
        {
            // �ر�fluent startup����
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

bool RunFluent( const CString& appDir )
{
    CString fluentExePath;
    if( !ReadFluentBinPath( fluentExePath ) )
    {
        AfxMessageBox( _T( "�밲װFluent���" ) );
        return false;
    }

    // ɾ����ʱ�ļ�
    RemoveFile( appDir + FLUENT_CAS_FILE );
    RemoveFile( appDir + FLUENT_DAT_FILE );
    RemoveFile( appDir + FLUENT_ERROR_FILE );

    // ����fluent
    MyFluentThreadData data = {NULL, NULL, appDir + FLUENT_DAT_FILE, appDir + FLUENT_ERROR_FILE};
    if( !RunFluent(
                fluentExePath,
                appDir,
                FLUENT_LOAD_SCM_FILE,
                data.hProcess,
                data.hThread,
                true ) )
    {
        AfxMessageBox( _T( "����fluentʧ��..." ) );
        return false;
    }

    // �����߳�
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