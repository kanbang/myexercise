#include "StdAfx.h"
#include "ArxLoader.h"

ArxLoader::ArxLoader( const CString& arxFilePath, const CString& serviceName )
    : m_arxFilePath( arxFilePath ), m_serviceName( serviceName )
{

}

bool ArxLoader::loadLibrary()
{
    TCHAR szPath[_MAX_PATH] = {0}, szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFile[_MAX_FNAME], szExt[_MAX_EXT];
    _tsplitpath( ( LPCTSTR )m_arxFilePath, szDrive, szDir, szFile, szExt ); // ���·��

    // �ж���չ���Ƿ�Ϊarx
    if( CString( szExt ) != _T( ".arx" ) ) return false;

    bool flag = false; // Ĭ��Ϊ����ʧ�ܣ�������ע������޷����أ�
    AcRxObject* pSvc;
    if ( !( pSvc = acrxServiceDictionary->at ( ( LPCTSTR )m_serviceName ) ) ) // ������δע��
    {
        flag = acrxDynamicLinker->loadModule ( ( LPCTSTR )m_arxFilePath, 0 );
    }
    return flag;
}

void ArxLoader::releaseLibrary()
{
    acrxDynamicLinker->unloadModule ( ( LPCTSTR )m_arxFilePath, 0 );
}

void* ArxLoader::getFunction( const CString& functionName )
{
    return acrxDynamicLinker->getSymbolAddress( ( LPCTSTR )m_serviceName, ( LPCTSTR )functionName );
}