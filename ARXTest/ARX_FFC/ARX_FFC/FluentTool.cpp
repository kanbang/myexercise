#include "StdAfx.h"

#include "FFC_Def.h"
#include "SysTool.h"

static CString TECPLOT_SHOW_TYPE_STRING_ARRAY[] =
{
    _T( "�ٶ���ͼ" ),
    _T( "�ٶ�ʸ��ͼ" ),
    _T( "�ٶ�����ͼ" ),
    _T( "�ٶ����߶���" ),
    _T( "��ѹ����ͼ" ),
    _T( "�¶���ͼ" ),
    _T( "�������������ֲ�" ),
    _T( "�������������ֲ�" ),
    _T( "�������������ֲ�" ),
    _T( "������̼���������ֲ�" ),
    _T( "һ����̼���������ֲ�" )
};

static CString GEO_ERROR_CODE_MSG[] =
{
    _T( "���ν�ģ��ȷ" ),
    _T( "ͼ����û�����/������" ),
    _T( "ͼ����û�й�����" ),
    _T( "ͼ����û�вɿ���" ),
    _T( "ͼ��û�й�������" ),
    _T( "û�й�������˹�߽�(������û����ɿ���������һ��)" ),
    _T( "û�й�����->�ɿ�����ѹ����(������û����ɿ���������һ��)" )
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

static void RemoveGambitTempFiles( const CString& appDir )
{
    RemoveFile( appDir + GAMBIT_JOU_FILE );
    RemoveFile( appDir + GAMBIT_TRN_FILE );
    RemoveFile( appDir + GAMBIT_DBS_FILE );
    RemoveFile( appDir + GAMBIT_LOK_FILE );
    RemoveFile( appDir + GAMBIT_ERROR_FILE );
}

static void RemoveGambitResultFiles( const CString& appDir )
{
    // ɾ������ļ�(msh)
    RemoveFile( appDir + GAMBIT_MESH_FILE );
}
static void RemoveFluentTempFiles( const CString& appDir )
{
    // ɾ����ʱ�ļ�
    RemoveFile( appDir + FLUENT_ERROR_FILE );
}

static void RemoveFluentResultFiles( const CString& appDir )
{
    // ɾ������ļ�(cas/dat)
    RemoveFile( appDir + FLUENT_CAS_FILE );
    RemoveFile( appDir + FLUENT_DAT_FILE );
}

static void RemoveTecplotTempFiles( const CString& appDir )
{
    RemoveFile( appDir + TECPLOT_PLOT_TYPE_FILE );
}

static void KillGambitProcess()
{
    KillProcess( _T( "gambit.exe" ) );
    KillProcess( _T( "exceed.exe" ) );
}

static void KillFluentProcess()
{
    KillProcess( _T( "fluent.exe" ) );
    KillProcess( _T( "fl6326s.exe" ) );
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

CString GetGambitErrorMessage( RUN_ERROR_CODE rst )
{
    CString msg;
    if( rst == REC_NOT_INSTALL )
    {
        msg = _T( "Gambit���û�а�װ" );
    }
    else if( rst == REC_PROCESS_ERROR )
    {
        msg = _T( "�޷�����Gambit����" );
    }
    else if( rst == REC_FILE_NOT_EXIST )
    {
        msg = _T( "�޷��ҵ������ļ�--GambitVar.jou" );
    }
    return msg;
}

CString GetFluentErrorMessage( RUN_ERROR_CODE rst )
{
    CString msg;
    if( rst == REC_NOT_INSTALL )
    {
        msg = _T( "Fluent���û�а�װ" );
    }
    else if( rst == REC_PROCESS_ERROR )
    {
        msg = _T( "�޷�����Fluent����" );
    }
    else if( rst == REC_FILE_NOT_EXIST )
    {
        msg = _T( "�޷��ҵ������ļ�--load.scm" );
    }
    return msg;
}

CString GetTecplotErrorMessage( RUN_ERROR_CODE rst )
{
    CString msg;
    if( rst == REC_NOT_INSTALL )
    {
        msg = _T( "Fluent���û�а�װ" );
    }
    else if( rst == REC_PROCESS_ERROR )
    {
        msg = _T( "�޷�����Fluent����" );
    }
    else if( rst == REC_FILE_NOT_EXIST )
    {
        msg = _T( "�޷��ҵ������ļ�--ffc.dat" );
    }
    return msg;
}

CString GetTecplotTypeName( TECPLOT_SHOW_TYPE type )
{
    return TECPLOT_SHOW_TYPE_STRING_ARRAY[type];
}

CString GetGeoErrorMessage( GEO_ERROR_CODE ge )
{
    return GEO_ERROR_CODE_MSG[ge];
}

void CleanWork( const CString& appDir )
{
    // kill���е�gambit����
    KillGambitProcess();

    // kill����fluent����
    KillFluentProcess();

    // ɾ����ʱ�ļ�
    RemoveGambitTempFiles( appDir );
    RemoveFluentTempFiles( appDir );
    RemoveTecplotTempFiles( appDir );

    // ɾ������ļ�
    RemoveGambitResultFiles( appDir );
    RemoveFluentResultFiles( appDir );
}