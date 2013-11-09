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

        // ��������2������ر��߳�:
        //	1) ������error�ļ������ʾ��������msh�ļ�
        //	2) ����msh�ļ������ʾ����msh�ļ��ɹ�
        if( IsRunError( pData ) || IsRunOK( pData ) )
        {
            // �ر�gambit startup����
            CloseGambitStartupWindow();
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

bool RunGambit( const CString& appDir )
{
    CString gambitExePath;
    if( !ReadGambitBinPath( gambitExePath ) )
    {
        AfxMessageBox( _T( "�밲װGambit���" ) );
        return false;
    }

    RemoveFile( appDir + GAMBIT_JOU_FILE );
    RemoveFile( appDir + GAMBIT_TRN_FILE );
    RemoveFile( appDir + GAMBIT_DBS_FILE );
    RemoveFile( appDir + GAMBIT_ERROR_FILE );

    // ����gambit
    MyGambitThreadData data = {NULL, NULL, appDir + GAMBIT_MESH_FILE, appDir + GAMBIT_ERROR_FILE};
    if( !RunGambit(
                gambitExePath,
                appDir,
                GAMBIT_VAR_JOU_FIEL_NAME,
                data.hProcess,
                data.hThread,
                true ) )
    {
        AfxMessageBox( _T( "����gambitʧ��..." ) );
        return false;
    }

    // �����߳�
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