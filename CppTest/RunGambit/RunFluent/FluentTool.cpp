#include "StdAfx.h"

#include "FluentTool.h"

static CString TECPLOT_SHOW_TYPE_STRING_ARRAY[] =
{
    _T( "�ٶ���ͼ" ),
    _T( "�ٶ�ʸ��ͼ" ),
    _T( "��ѹ����ͼ" ),
    _T( "�ٶ����߶���" ),
    _T( "�������������ֲ�" ),
    _T( "�������������ֲ�" ),
    _T( "�������������ֲ�" )
};

// ��ȡgambit��װ·��
static bool ReadGambitInstallPath( CString& path )
{
    long lRet = 0;
    HKEY hResult;
    lRet = ::RegOpenKeyEx( GAMBIT_KEY, GAMBIT_SUB_KEY, 0, KEY_QUERY_VALUE, &hResult );
    if( lRet == ERROR_SUCCESS )
    {
        DWORD      WordType =  REG_SZ;
        DWORD      WordData =  100;
        TCHAR     KeyByte[1024];
        lRet = ::RegQueryValueEx( hResult, NULL, NULL, &WordType, ( LPBYTE )KeyByte, &WordData );
        if( lRet == ERROR_SUCCESS )
        {
            path = KeyByte;
        }
        RegCloseKey( hResult );
    }
    return ( lRet == ERROR_SUCCESS );
}

bool ReadGambitBinPath( CString& path )
{
    CString installPath;
    if( !ReadGambitInstallPath( installPath ) ) return false;

    path.Format( _T( "%s%s" ), installPath, GAMBIT_BIN );
    return true;
}

void CloseGambitStartupWindow()
{
    HWND hwnd = FindWindow( NULL, GAMBIT_STARTUP_WINDOWS_NAME );
    if( hwnd != NULL )
    {
        // ����close��ť
        HWND bhwnd = FindWindowEx( hwnd, NULL, _T( "Button" ), _T( "Close" ) );
        if( bhwnd != NULL )
        {
            ::PostMessage( hwnd, WM_SYSCOMMAND, SC_CLOSE, 0 );
            //::PostMessage(hwnd, WM_CLOSE, 0, 0);
        }
    }
}

// ��ȡFluent��װ·��
static bool ReadFluentInstallPath( CString& path )
{
    long lRet = 0;
    HKEY hResult;
    lRet = ::RegOpenKeyEx( FLUENT_KEY, FLUENT_SUB_KEY, 0, KEY_QUERY_VALUE, &hResult );
    if( lRet == ERROR_SUCCESS )
    {
        DWORD      WordType =  REG_SZ;
        DWORD      WordData =  100;
        TCHAR     KeyByte[1024];
        lRet = ::RegQueryValueEx( hResult, NULL, NULL, &WordType, ( LPBYTE )KeyByte, &WordData );
        if( lRet == ERROR_SUCCESS )
        {
            path = KeyByte;
        }
        RegCloseKey( hResult );
    }
    return ( lRet == ERROR_SUCCESS );
}

bool ReadFluentBinPath( CString& path )
{
    CString installPath;
    if( !ReadFluentInstallPath( installPath ) ) return false;

    path.Format( _T( "%s%s" ), installPath, FLUENT_BIN );
    return true;
}

bool RunGambit( const CString& gambitExePath, const CString& cwdPath, const CString& jouFile, HANDLE& hProcess, HANDLE& hThread, bool hidden )
{
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    memset( &si, 0, sizeof( si ) );
    si.cb = sizeof( si );
    si.wShowWindow = hidden ? SW_HIDE : SW_SHOW;
    si.dwFlags = STARTF_USESHOWWINDOW;

    CString cmdLine;
    cmdLine.Format( _T( "-r2.2.30 -id %s -inputfile %s" ), GAMBIT_SESSION_ID, jouFile );

    BOOL ret = CreateProcess( ( LPCTSTR )gambitExePath, ( LPTSTR )( LPCTSTR )cmdLine, NULL, FALSE, NULL, NULL, NULL, ( LPCTSTR )cwdPath, &si, &pi );
    if( ret )
    {
        //WaitForSingleObject(pi.hProcess, INFINITE);
        //CloseHandle(pi.hThread);
        //CloseHandle(pi.hProcess);

        // ���ؽ��̺��߳̾��
        hProcess = pi.hProcess;
        hThread = pi.hThread;
    }
    return ( ret == TRUE );
}

bool RunFluent( const CString& fluentExePath, const CString& cwdPath, const CString& scmFile, HANDLE& hProcess, HANDLE& hThread, bool hidden )
{
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    memset( &si, 0, sizeof( si ) );
    si.cb = sizeof( si );
    si.wShowWindow = hidden ? SW_HIDE : SW_SHOW;
    si.dwFlags = STARTF_USESHOWWINDOW;

    CString cmdLine;
    if( hidden )
    {
        cmdLine.Format( _T( " -r6.3.26 2d -hidden -i %s" ), scmFile );
    }
    else
    {
        cmdLine.Format( _T( " -r6.3.26 2d -i %s" ), scmFile );
    }

    BOOL ret = CreateProcess( ( LPCTSTR )fluentExePath, ( LPTSTR )( LPCTSTR )cmdLine, NULL, FALSE, NULL, NULL, NULL, ( LPCTSTR )cwdPath, &si, &pi );
    if( ret )
    {
        //WaitForSingleObject(pi.hProcess, INFINITE);
        //CloseHandle(pi.hThread);
        //CloseHandle(pi.hProcess);

        // ���ؽ��̺��߳̾��
        hProcess = pi.hProcess;
        hThread = pi.hThread;
    }
    return ( ret == TRUE );
}

CString GetTecplotType( TECPLOT_SHOW_TYPE type )
{
    return TECPLOT_SHOW_TYPE_STRING_ARRAY[type];
}