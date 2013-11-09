#include "stdafx.h"
#include <tlhelp32.h>

#include <vector>

// ��ȡ��ǰģ���·��
CString GetAppPathDir()
{
    TCHAR szMoudlePath[_MAX_PATH];
    GetModuleFileName( _hdllInstance, szMoudlePath, _MAX_PATH );

    TCHAR drive[_MAX_DRIVE];
    TCHAR dir[_MAX_DIR];
    _tsplitpath( szMoudlePath, drive, dir, NULL, NULL );

    TCHAR path[_MAX_PATH];
    _tmakepath( path, drive, dir, NULL, NULL );

    return CString( path );
}

// ����·��
CString BuildPath( const CString& dir, const CString& fileName )
{
    CString path;
    path.Format( _T( "%s%s" ), dir, fileName );
    return path;
}

void RemoveFile( const CString& filePath )
{
    CFileFind ff;
    if( ff.FindFile( filePath ) )
    {
        CFile::Remove( filePath );
    }
}

bool IsFileExist( const CString& filePath )
{
    CFileFind ff;
    return ff.FindFile( filePath );
}

// http://www.cnblogs.com/phinecos/archive/2008/06/19/1225224.html
static bool CreateMultipleDirectory( const CString& szPath )
{
    CString strDir( szPath ); //���Ҫ������Ŀ¼�ַ���
    //ȷ����'\'��β�Դ������һ��Ŀ¼
    if ( strDir.GetAt( strDir.GetLength() - 1 ) != _T( '\\' ) )
    {
        strDir.AppendChar( _T( '\\' ) );
    }
    std::vector<CString> vPath;//���ÿһ��Ŀ¼�ַ���
    CString strTemp;//һ����ʱ����,���Ŀ¼�ַ���
    bool bSuccess = false;//�ɹ���־
    //����Ҫ�������ַ���
    for ( int i = 0; i < strDir.GetLength(); ++i )
    {
        if ( strDir.GetAt( i ) != _T( '\\' ) )
        {
            //�����ǰ�ַ�����'\\'
            strTemp.AppendChar( strDir.GetAt( i ) );
        }
        else
        {
            //�����ǰ�ַ���'\\'
            vPath.push_back( strTemp ); //����ǰ����ַ�����ӵ�������
            strTemp.AppendChar( _T( '\\' ) );
        }
    }

    //�������Ŀ¼������,����ÿ��Ŀ¼
    std::vector<CString>::const_iterator vIter;
    for ( vIter = vPath.begin(); vIter != vPath.end(); vIter++ )
    {
        //���CreateDirectoryִ�гɹ�,����true,���򷵻�false
        bSuccess = CreateDirectory( *vIter, NULL ) ? true : false;
    }

    return bSuccess;
}

void CreateDataDirectory( const CString& data_dir )
{
    //if(CreateDirectory(data_dir, NULL))
    if( CreateMultipleDirectory( data_dir ) )
    {
        acutPrintf( _T( "\n����·��%s�ɹ�" ), data_dir );
    }
    else
    {
        acutPrintf( _T( "\n����·��%sʧ��" ), data_dir );
    }
}

static void FindProcessIdByName( const CString& name, std::vector<DWORD>& pids )
{
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( hProcessSnap == INVALID_HANDLE_VALUE )
    {
        return ;
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof( PROCESSENTRY32 );

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if( !Process32First( hProcessSnap, &pe32 ) )
    {
        CloseHandle( hProcessSnap );    // Must clean up the
        //   snapshot object!
        return ;
    }

    DWORD pid = -1;
    // Now walk the snapshot of processes, and
    // display information about each process in turn
    do
    {
        if( name.CompareNoCase( pe32.szExeFile ) == 0 )
        {
            pids.push_back( pe32.th32ProcessID );
        }

    }
    while( Process32Next( hProcessSnap, &pe32 ) );

    CloseHandle( hProcessSnap );

    //return pid;
}

void KillProcess( const CString& name )
{
    std::vector<DWORD> pids;
    FindProcessIdByName( name, pids );

    int n = pids.size();
    for( int i = 0; i < n; i++ )
    {
        HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pids[i] );
        if( hProcess != NULL )
        {
            // 4����ʲô��˼???
            TerminateProcess( hProcess, 4 );
            CloseHandle( hProcess );
        }
    }
}

bool IsProcessActive( const CString& name )
{
    std::vector<DWORD> pids;
    FindProcessIdByName( name, pids );
    return !pids.empty();
}