#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <io.h>
using namespace std;

#define GAMBIT_VAR_JOU_FIEL_NAME "GambitVar.jou"
#define GAMBIT_STARTUP_WINDOWS_NAME "Gambit Startup"

#define GAMBIT_SESSION_ID "ffc"
#define GAMBIT_MESH_FILE "ffc.msh"
#define GAMBIT_JOU_FILE "ffc.jou"
#define GAMBIT_TRN_FILE "ffc.trn"
#define GAMBIT_DBS_FILE "ffc.dbs"

#define THREAD_CHECK_TIME_INTERVAL 1000

string GetAppPathDir()
{
    char szMoudlePath[_MAX_PATH];
    //GetModuleFileName(GetModuleHandle("VNGGen.dll"), szMoudlePath, _MAX_PATH);
    GetModuleFileName( NULL, szMoudlePath, _MAX_PATH );

    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    _splitpath( szMoudlePath, drive, dir, NULL, NULL );

    char path[_MAX_PATH];
    _makepath( path, drive, dir, NULL, NULL );

    return string( path );
}

string BuildPath( const string& dir, const string& fileName )
{
    stringstream ss;
    ss << dir << fileName;
    return ss.str();
}

void RemoveFile( const string& filePath )
{
    // 在<stdio.h>中提供
    remove( filePath.c_str() );
}

bool IsFileExist( const char* filename )
{
    // 在<io.h>中提供
    return ( access( filename, 0 ) == 0 );

    // 在<shlwapi.h>中提供
    //return PathFileExists(filename);
}

// 读取gambit安装路径
static bool ReadGambitInstallPath( string& path )
{
    long lRet = 0;
    string strRegGV = "Software\\Fluent Inc Products\\Gambit"; // gambit
    HKEY hResult;
    lRet = ::RegOpenKeyEx( HKEY_CURRENT_USER, strRegGV.c_str(), 0, KEY_QUERY_VALUE, &hResult );
    if( lRet == ERROR_SUCCESS )
    {
        DWORD      WordType =  REG_SZ;
        DWORD      WordData =  100;
        char     KeyByte[1024];
        lRet = ::RegQueryValueEx( hResult, NULL, NULL, &WordType, ( LPBYTE )KeyByte, &WordData );
        if( lRet == ERROR_SUCCESS )
        {
            path = KeyByte;

            // 合并路径
            stringstream ss;
            ss << path << "ntbin\\ntx86\\gambit.exe";
            path = ss.str();
        }
        RegCloseKey( hResult );
    }
    return ( lRet == ERROR_SUCCESS );
}

static void CloseGambitStartup()
{
    HWND hwnd = FindWindow( NULL, GAMBIT_STARTUP_WINDOWS_NAME );
    if( hwnd != NULL )
    {
        cout << "查找gambit startup窗口句柄成功" << endl;

        // 查找close按钮
        HWND bhwnd = FindWindowEx( hwnd, NULL, "Button", "Close" );

        if( bhwnd != NULL )
        {
            cout << "查找close按钮成功" << endl;
            ::PostMessage( hwnd, WM_SYSCOMMAND, SC_CLOSE, 0 );
            //::PostMessage(hwnd, WM_CLOSE, 0, 0);
        }
        else
        {
            cout << "查找close按钮失败" << endl;
        }
    }
    else
    {
        cout << "查找gambit startup窗口句柄失败" << endl;
    }
}

static bool RunGambit( const string& gambitExePath, const string& cwdPath, const string& jouFileName, HANDLE& hProcess, HANDLE& hThread )
{
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    memset( &si, 0, sizeof( si ) );
    si.cb = sizeof( si );
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW;

    stringstream ss;
    ss << "-r2.2.30 -id " << GAMBIT_SESSION_ID << " -inputfile " << jouFileName;
    string cmdLine = ss.str();

    cout << "gambit路径:" << gambitExePath << endl;
    cout << "参数: " << cmdLine << endl;

    BOOL ret = CreateProcess( ( char* )gambitExePath.c_str(), ( char* )cmdLine.c_str(), NULL, FALSE, NULL, NULL, NULL, cwdPath.c_str(), &si, &pi );
    if( ret )
    {
        //WaitForSingleObject(pi.hProcess, INFINITE);
        //CloseHandle(pi.hThread);
        //CloseHandle(pi.hProcess);

        // 返回进程和线程句柄
        hProcess = pi.hProcess;
        hThread = pi.hThread;
    }
    return ( ret == TRUE );
}

struct MyThreadData
{
    HANDLE hThread;
    HANDLE hProcess;
    string mshFilePath;
};

DWORD MyThread( LPVOID lpParam )
{
    // 在线程中检查是否存在
    MyThreadData* pData = ( MyThreadData* )lpParam;

    while( true )
    {
        Sleep( THREAD_CHECK_TIME_INTERVAL );
        cout << "等待" << THREAD_CHECK_TIME_INTERVAL / 1000 << "s执行" << endl;

        // 如果文件存在，则发送消息
        if( IsFileExist( pData->mshFilePath.c_str() ) )
        {
            cout << "文件" << pData->mshFilePath << "被发现" << endl;

            // 关闭gambit startup窗口
            CloseGambitStartup();
            CloseHandle( pData->hThread );
            CloseHandle( pData->hProcess );

            //DWORD exitCode;
            //GetExitCodeProcess(pData->hProcess, &exitCode);
            //ExitProcess(exitCode);

            break;
        }
    }
    return 0;
}

int main()
{
    string gambitExePath;
    if( !ReadGambitInstallPath( gambitExePath ) )
    {
        cout << "请安装Gambit/Fluent软件" << endl;
        return 0;
    }

    string appDir = GetAppPathDir();
    cout << "当前目录:" << appDir << endl;

    cout << "删除上次一生成的临时文件" << endl;
    RemoveFile( appDir + GAMBIT_JOU_FILE );
    RemoveFile( appDir + GAMBIT_TRN_FILE );
    RemoveFile( appDir + GAMBIT_DBS_FILE );

    // 启动gambit
    cout << "创建gambit进程，执行jou文件" << endl;
    MyThreadData data = {NULL, NULL, appDir + GAMBIT_MESH_FILE};
    if( !RunGambit( gambitExePath, appDir, GAMBIT_VAR_JOU_FIEL_NAME, data.hProcess, data.hThread ) )
    {
        cout << "启动gambit失败..." << endl;
        return 0;
    }

    // 创建线程
    cout << "创建线程，监视msh文件是否存在" << endl;
    DWORD dwThreadId;
    HANDLE hThread = CreateThread(
                         NULL,              // default security attributes
                         0,                 // use default stack size
                         ( LPTHREAD_START_ROUTINE )MyThread,        // thread function
                         &data,             // argument to thread function
                         0,                 // use default creation flags
                         &dwThreadId );  // returns the thread identifier

    WaitForSingleObject( hThread, INFINITE );
    CloseHandle( hThread );

    //cout<<"test run gambit"<<endl;

    return 0;
}
