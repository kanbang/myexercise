#include "StdAfx.h"
#include "Utils.h"

// 获取当前模块的路径
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

// 生成路径
CString BuildPath( const CString& dir, const CString& fileName )
{
    CString path;
    path.Format( _T( "%s%s" ), dir, fileName );
    return path;
}

CString BuildArxFileName( const CString& arxModuleName )
{
    CString str;
    str.Format( _T( "%s.arx" ), arxModuleName );
    return str;
}

CString BuildServiceName( const CString& arxModuleName )
{
    CString str;
    str.Format( _T( "%s_SERVICE_NAME" ), arxModuleName );
    return str.MakeUpper();
}
