#include "StdAfx.h"

#include "FFC_Def.h"
#include "SysTool.h"
#include "FluentTool.h"

bool RunTecplot( const CString& appDir, int type )
{
    CString fluentExePath;
    if( !ReadFluentBinPath( fluentExePath ) )
    {
        AfxMessageBox( _T( "请安装Fluent软件" ) );
        return false;
    }

    // 将当前路径调整到plot目录下
    CString dir = appDir + TECPLOT_DATA_DIR;

    CString scmFile;
    scmFile.Format( _T( "%s%s.scm" ), dir, GetTecplotType( ( TECPLOT_SHOW_TYPE )type ) );

    // 启动fluent
    HANDLE hThread;
    HANDLE hProcess;
    if( !RunFluent( fluentExePath, dir, scmFile, hProcess, hThread, false ) )
    {
        AfxMessageBox( _T( "启动fluent失败..." ) );
        return false;
    }

    return true;
}