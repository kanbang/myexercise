#include "StdAfx.h"
#include "FilePathHelper.h"

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

CString BuildPath( const CString& dir, const CString& fileName )
{
    CString path;
    path.Format( _T( "%s%s" ), dir, fileName );
    return path;
}

CString GetPathDir( const CString& path )
{
    // 方法1，使用splitpath和makepath方法(系统api）
    /*
    char path_buffer[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    // 拆分路径
    _splitpath(	m_outputEvalReportPath.GetBuffer(),
    drive, dir, fname, ext);
    m_outputEvalReportPath.ReleaseBuffer();

    // 合并得到ini文件的路径
    _makepath_s(path_buffer, drive, dir, fname, ".ini");
    */

    // 方法2：也可以使用cstring的方法
    return path.Left( path.ReverseFind( _T( '\\' ) ) );
}

bool CreateEmptyFile( const CString& path )
{
    CFile myfile;
    CFileException fileException;
    if( !myfile.Open( path, CFile::modeCreate | CFile::modeWrite, &fileException ) ) // 新建文件
    {
        // 生成数据文件失败
        /*CString msg;
        msg.Format("生成数据文件失败!\n文件: %s，错误: %u", path, fileException.m_cause);
        AfxMessageBox(msg);*/
        return false;
    }
    else
    {
        myfile.Close(); // 关闭文件
        return true;
    }
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

CDaoDatabase* CreateDB( const CString& dbName, bool openExist )
{
    CDaoDatabase* pDB = new CDaoDatabase();
    CFileFind ff;
    if( ff.FindFile( dbName ) )
    {
        if( openExist )
        {
            pDB->Open( dbName );
        }
        else
        {
            CFile::Remove( dbName );
            pDB->Create( dbName );
        }
    }
    else
    {
        pDB->Create( dbName );
    }
    return pDB;
}