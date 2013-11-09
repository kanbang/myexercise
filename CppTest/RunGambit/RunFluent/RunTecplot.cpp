#include "StdAfx.h"

#include "FFC_Def.h"
#include "SysTool.h"
#include "FluentTool.h"

bool RunTecplot( const CString& appDir, int type )
{
    CString fluentExePath;
    if( !ReadFluentBinPath( fluentExePath ) )
    {
        AfxMessageBox( _T( "�밲װFluent���" ) );
        return false;
    }

    // ����ǰ·��������plotĿ¼��
    CString dir = appDir + TECPLOT_DATA_DIR;

    CString scmFile;
    scmFile.Format( _T( "%s%s.scm" ), dir, GetTecplotType( ( TECPLOT_SHOW_TYPE )type ) );

    // ����fluent
    HANDLE hThread;
    HANDLE hProcess;
    if( !RunFluent( fluentExePath, dir, scmFile, hProcess, hThread, false ) )
    {
        AfxMessageBox( _T( "����fluentʧ��..." ) );
        return false;
    }

    return true;
}