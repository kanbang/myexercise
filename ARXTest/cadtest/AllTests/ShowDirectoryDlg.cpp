#include "stdafx.h"


// 删除路径后面的斜杠(slash)
static void RemoveSlash( CString& dirPath )
{
    if( dirPath != _T( "" ) )
    {
        if( dirPath.Right( 1 ) == _T( "\\" ) )
        {
            dirPath = dirPath.Left( dirPath.GetLength() - 1 );
        }
    }
}

static void AddSlash( CString& dirPath )
{
    if( dirPath != _T( "" ) )
    {
        if( dirPath.Right( 1 ) != _T( "\\" ) )
        {
            dirPath += _T( "\\" );
            // 在尾部附加"\"
        }
    }
}

bool ShowDirectoryDlg( const CString& initPath, CString& dirPath )
{
    BROWSEINFO bInfo;
    LPITEMIDLIST lpidBrowse;

    CString szDir = initPath;
    RemoveSlash( szDir );
    if( szDir == _T( "" ) ) return false;

    bInfo.hwndOwner = AcUiMainWindow()->m_hWnd; // cad主窗口
    bInfo.pidlRoot = 0;

    bInfo.pszDisplayName = 0;
    bInfo.lpszTitle = _T( "请选择文件夹" );
    bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_USENEWUI;

    bInfo.lpfn = 0;
    bInfo.lParam = 0;
    bInfo.iImage = 0;

    lpidBrowse = SHBrowseForFolder( &bInfo );
    if( lpidBrowse == 0 ) return false;

    TCHAR lpszPath[_MAX_PATH];
    if( !SHGetPathFromIDList( lpidBrowse, lpszPath ) ) return false;

    dirPath = lpszPath;
    AddSlash( dirPath );

    return true;
}