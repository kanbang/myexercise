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
    // ����1��ʹ��splitpath��makepath����(ϵͳapi��
    /*
    char path_buffer[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    // ���·��
    _splitpath(	m_outputEvalReportPath.GetBuffer(),
    drive, dir, fname, ext);
    m_outputEvalReportPath.ReleaseBuffer();

    // �ϲ��õ�ini�ļ���·��
    _makepath_s(path_buffer, drive, dir, fname, ".ini");
    */

    // ����2��Ҳ����ʹ��cstring�ķ���
    return path.Left( path.ReverseFind( _T( '\\' ) ) );
}

bool CreateEmptyFile( const CString& path )
{
    CFile myfile;
    CFileException fileException;
    if( !myfile.Open( path, CFile::modeCreate | CFile::modeWrite, &fileException ) ) // �½��ļ�
    {
        // ���������ļ�ʧ��
        /*CString msg;
        msg.Format("���������ļ�ʧ��!\n�ļ�: %s������: %u", path, fileException.m_cause);
        AfxMessageBox(msg);*/
        return false;
    }
    else
    {
        myfile.Close(); // �ر��ļ�
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