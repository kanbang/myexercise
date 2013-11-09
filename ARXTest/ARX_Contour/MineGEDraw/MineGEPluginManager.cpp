#include "StdAfx.h"
#include "MineGEPluginManager.h"
#include "MineGEPlugin.h"
#include "MineGEDrawManager.h"
#include "ARXLoader.h"
#include "MineGEPlugin.h"

MineGEPluginManager::MineGEPluginManager( MineGEDrawManager* pMineGEDrawManager ) : m_pMineGEDrawManager( pMineGEDrawManager )
{
}

MineGEPluginManager::~MineGEPluginManager( void )
{
    unloadAllPlugins();
}

ArxLoader* MineGEPluginManager::loadARXModule( const CString& arxFilePath, const CString& serviceName )
{
    ArxLoader* pARXLoader = new ArxLoader( arxFilePath, serviceName );

    if( !pARXLoader->loadLibrary() ) 	          // ���ز��ʧ��
    {
        delete pARXLoader;
        return 0;
    }

    return pARXLoader;
}

MineGEPlugin* MineGEPluginManager::createMineGEPlugin( ArxLoader* pARXLoader )
{
    MineGEPluginCreateFunc mgdpc = 		        // ���Ҵ�������
        ( MineGEPluginCreateFunc )pARXLoader->getFunction( SYMBOL_STRINGIZE( MINEGE_PLUGIN_CREATE_FUNCTION_NAME ) );

    if( mgdpc == 0 ) return 0;

    // �����������
    return ( *mgdpc )();
}

void MineGEPluginManager::load( const CString& arxFilePath, const CString& serviceName )
{
    ArxLoader* pARXLoader = loadARXModule( arxFilePath, serviceName );
    if( pARXLoader == 0 )
    {
        acutPrintf( _T( "\n���ز��:\n\t%sʧ��\n\tע���������:%s" ), arxFilePath, serviceName );
        return;
    }

    MineGEPlugin* pMineGEPlugin = createMineGEPlugin( pARXLoader );
    if( pMineGEPlugin == 0 )
    {
        acutPrintf( _T( "\n��ʼ�����:\n\t%sʧ��\n\tע���������:%s" ), arxFilePath, serviceName );
        pARXLoader->releaseLibrary();
        delete pARXLoader;
        return;
    }

    acutPrintf( _T( "\n�ɹ����ز��\n\t%s\n\tע���������:%s" ), arxFilePath, serviceName );

    // �����ʼ��
    pMineGEPlugin->initialize( m_pMineGEDrawManager );

    // ���������Ϣ������ӵ��������
    PluginInfo info;
    info.pluginFilePath = arxFilePath;
    info.pARXLoader = pARXLoader;
    info.pMineGEPlugin = pMineGEPlugin;

    m_plugins.push_back( info );
}

void MineGEPluginManager::unload( const CString& arxFilePath )
{
    for ( Plugins::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it )
    {
        if ( arxFilePath.CompareNoCase( ( *it ).pluginFilePath ) == 0 )
        {
            unload( *it );
            m_plugins.erase( it );
            break;
        }
    }
}

void MineGEPluginManager::unload( PluginInfo& info )
{
    info.pMineGEPlugin->unInitialize( m_pMineGEDrawManager ); // write

    info.pARXLoader->releaseLibrary();
    delete info.pARXLoader;
    // ������delete pMineGEPlugin����Ϊ�����ڴ�����dllͨ����̬�����ķ�ʽ�����
    //delete info.pMineGEPlugin;
}

void MineGEPluginManager::unloadAllPlugins()
{
    for ( Plugins::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it )
    {
        unload( *it );
    }
}