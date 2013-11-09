#include "StdAfx.h"
#include "ArxLoader.h"

namespace PJ
{
    ArxLoader::ArxLoader( const CString& arxFilePath, const CString& serviceName )
        : m_arxFilePath( arxFilePath ), m_serviceName( serviceName )
    {

    }

    bool ArxLoader::loadLibrary()
    {
        TCHAR szDrive[_MAX_DRIVE];
        TCHAR szDir[_MAX_DIR];
        TCHAR szFile[_MAX_FNAME];
        TCHAR szExt[_MAX_EXT];
        _tsplitpath( ( LPCTSTR )m_arxFilePath, szDrive, szDir, szFile, szExt ); // 拆分路径

        // 判断扩展名是否为arx
        if( CString( szExt ).CompareNoCase( _T( ".arx" ) ) != 0 ) return false;

        bool flag = true;
        AcRxObject* pSvc = acrxServiceDictionary->at( ( LPCTSTR )m_serviceName );
        if( pSvc == NULL ) // 服务尚未注册
        {
            flag = acrxDynamicLinker->loadModule( ( LPCTSTR )m_arxFilePath, false );
        }
        return flag;
    }

    void ArxLoader::releaseLibrary()
    {
        acrxDynamicLinker->unloadModule( ( LPCTSTR )m_arxFilePath, 0 );
    }

    void* ArxLoader::getFunction( const CString& functionName )
    {
        return acrxDynamicLinker->getSymbolAddress( ( LPCTSTR )m_serviceName, ( LPCTSTR )functionName );
    }
}
