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

#import "acax18enu.tlb" no_implementation raw_interfaces_only named_guids

static void SplitCString( const CString& content, const CString& token, AcStringArray& cc )
{
    int curPos = 0;
    CString str = content.Tokenize( token, curPos );
    while ( str != _T( "" ) )
    {
        cc.append( str );
        str = content.Tokenize( token, curPos );
    };
}

static bool IsPathExit( const CString& supportPath, const CString& arxPath )
{
    AcStringArray cc;
    SplitCString( supportPath, _T( ";" ), cc );

    bool bFind = false;
    int n = cc.length();
    for( int i = 0; i < n; i++ )
    {
        if( cc[i].compareNoCase( arxPath ) == 0 )
        {
            bFind = true;
            break;
        }
    }
    return bFind;
}

static CString ExcludePath( const CString& supportPath, const CString& arxPath )
{
    AcStringArray cc;
    SplitCString( supportPath, _T( ";" ), cc );

    AcStringArray paths;

    int n = cc.length();
    for( int i = 0; i < n; i++ )
    {
        if( cc[i].compareNoCase( arxPath ) == 0 ) continue;
        paths.append( cc[i] );
    }

    CString path;
    n = paths.length();
    for( int i = 0; i < n; i++ )
    {
        if( i == n - 1 )
        {
            path.AppendFormat( _T( "%s" ), paths[i].kACharPtr() );
        }
        else
        {
            path.AppendFormat( _T( "%s;" ), paths[i].kACharPtr() );
        }
    }
    return path;
}

bool loadArxModule( const CString& arxModuleName )
{
    CString arxName = BuildArxFileName( arxModuleName );
    CString serviceName = BuildServiceName( arxModuleName );

    AcRxObject* pSvc = acrxServiceDictionary->at ( serviceName );
    if( pSvc == NULL )
    {
        if( !acrxDynamicLinker->loadModule( arxName, false ) )
        {
            acutPrintf ( _T( "\n加载%s模块失败...\n" ), arxName );
            return false;
        }
        acutPrintf ( _T( "\n成功加载%s模块...\n" ), arxName );
    }
    else
    {
        acutPrintf ( _T( "\n%s模块已经加载过了...\n" ), arxName );
    }
    return true;
}

void unloadArxModule( const CString& arxModuleName )
{
    acrxDynamicLinker->unloadModule( BuildArxFileName( arxModuleName ), 0 );
    acutPrintf( _T( "\n卸载模块:%s\n" ), arxModuleName );
}

bool AddSupportPath( const CString& arxPath )
{
    AutoCAD::IAcadPreferences* pPrefer;
    AutoCAD::IAcadPreferencesFiles* pPreferFiles;
    HRESULT hr = NOERROR;

    bool ret = true;
    TRY
    {
        LPDISPATCH pAcadDisp = acedGetAcadWinApp()->GetIDispatch( TRUE );
        AutoCAD::IAcadApplication* pAcad = NULL;
        if ( SUCCEEDED( pAcadDisp->QueryInterface( AutoCAD::IID_IAcadApplication, ( void** )&pAcad ) ) )
        {
            BSTR supportPath;
            hr = pAcad->get_Preferences( &pPrefer );
            pAcad->Release();

            hr = pPrefer->get_Files( &pPreferFiles );
            pPrefer->Release();

            hr = pPreferFiles->get_SupportPath( &supportPath );
            CString stemp = supportPath;
            if( !IsPathExit( stemp, arxPath ) )
            {
                stemp = stemp + _T( ";" ) + arxPath;
                supportPath = stemp.AllocSysString();
                pPreferFiles->put_SupportPath( supportPath );
            }
            SysFreeString( supportPath ); // 用完释放
        }
        pPreferFiles->Release();
    }
    CATCH( COleDispatchException, e )
    {
        //e->ReportError(); // 调试用
        e->Delete();
        ret = false;
    }

    END_CATCH;

    return ret;
}

bool RemoveSupportPath( const CString& arxPath )
{
    AutoCAD::IAcadPreferences* pPrefer;
    AutoCAD::IAcadPreferencesFiles* pPreferFiles;
    HRESULT hr = NOERROR;

    bool ret = true;
    TRY
    {
        LPDISPATCH pAcadDisp = acedGetAcadWinApp()->GetIDispatch( TRUE );
        AutoCAD::IAcadApplication* pAcad = NULL;
        if ( SUCCEEDED( pAcadDisp->QueryInterface( AutoCAD::IID_IAcadApplication, ( void** )&pAcad ) ) )
        {
            BSTR supportPath;
            hr = pAcad->get_Preferences( &pPrefer );
            pAcad->Release();

            hr = pPrefer->get_Files( &pPreferFiles );
            pPrefer->Release();

            hr = pPreferFiles->get_SupportPath( &supportPath );

            // 排除重复路径
            CString stemp = supportPath;
            stemp = ExcludePath( stemp, arxPath );
            supportPath = stemp.AllocSysString();
            pPreferFiles->put_SupportPath( supportPath );
            SysFreeString( supportPath ); // 用完释放
        }
        pPreferFiles->Release();
    }
    CATCH( COleDispatchException, e )
    {
        //e->ReportError(); // 调试用
        e->Delete();
        ret = false;
    }
    END_CATCH;

    return ret;
}
