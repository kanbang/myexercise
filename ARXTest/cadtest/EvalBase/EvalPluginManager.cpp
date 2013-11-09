#include "StdAfx.h"
#include "EvalPluginManager.h"
#include "EvalMethodManager.h"
#include "EvalPlugin.h"
#include "ARXLoader.h"
#include "HelperMacro.h"

namespace PJ
{
    EvalPluginManager::EvalPluginManager( EvalMethodManager* pEvalMethodManager ) : m_pEvalMethodManager( pEvalMethodManager )
    {
    }

    EvalPluginManager::~EvalPluginManager( void )
    {
        unloadAllPlugins();
    }

    ArxLoader* EvalPluginManager::loadARXModule( const CString& arxFilePath, const CString& serviceName )
    {
        ArxLoader* pARXLoader = new ArxLoader( arxFilePath, serviceName );

        if( !pARXLoader->loadLibrary() ) 	          // ���ز��ʧ��
        {
            delete pARXLoader;
            return 0;
        }

        return pARXLoader;
    }

    EvalPlugin* EvalPluginManager::createEvalPlugin( ArxLoader* pARXLoader )
    {
        EvalPluginCreateFunc epcf = ( EvalPluginCreateFunc )pARXLoader->getFunction( EVAL_SYMBOL_STRINGIZE( EVAL_PLUGIN_CREATE_FUNCTION_NAME ) );

        if( epcf == 0 ) return 0;

        // �����������
        return ( *epcf )();
    }

    void EvalPluginManager::load( const CString& arxFilePath, const CString& serviceName )
    {
        ArxLoader* pARXLoader = loadARXModule( arxFilePath, serviceName );
        if( pARXLoader == 0 )
        {
            acutPrintf( _T( "\n���ز��%sʧ�ܣ�ע���������:%s" ), arxFilePath, serviceName );
            return;
        }

        EvalPlugin* pEvalPlugin = createEvalPlugin( pARXLoader );
        if( pEvalPlugin == 0 )
        {
            acutPrintf( _T( "\n��ʼ�����%sʧ�ܣ�ע���������:%s" ), arxFilePath, serviceName );
            return;
        }

        acutPrintf( _T( "\n�ɹ����ز��%s��ע���������:%s" ), arxFilePath, serviceName );

        EvalPluginInfo epi;
        pEvalPlugin->getPluginInfo( epi );
        CString regulationName = epi.name;
        m_pEvalMethodManager->regEvalMethod( regulationName );
        pEvalPlugin->initialize( m_pEvalMethodManager->getEvalMethodByName( regulationName ) );

        PluginInfo info;
        info.arxFilePath = arxFilePath;
        info.pARXLoader = new ArxLoader( arxFilePath, serviceName );
        info.pEvalPlugin = pEvalPlugin;

        m_plugins.append( info );
    }

    void EvalPluginManager::unload( const CString& arxFilePath )
    {
        int index = -1;
        int len = m_plugins.length();
        for( int i = 0; i < len; i++ )
        {
            if ( arxFilePath.CompareNoCase( m_plugins[i].arxFilePath ) == 0 )
            {
                index = i;
                break;
            }
        }
        if( index != -1 )
        {
            unload( m_plugins[index] );
            m_plugins.removeAt( index );
        }
    }

    void EvalPluginManager::unload( PluginInfo& info )
    {
        EvalPluginInfo epi;
        info.pEvalPlugin->getPluginInfo( epi );
        info.pEvalPlugin->unInitialize( m_pEvalMethodManager->getEvalMethodByName( epi.name ) );
        info.pARXLoader->releaseLibrary();
        delete info.pARXLoader;
        // ������delete pEvalPlugin, ��Ϊ�����ڴ�����dllͨ����̬�����ķ�ʽ�����
        //delete info.pEvalPlugin;
    }

    void EvalPluginManager::unloadAllPlugins()
    {
        int len = m_plugins.length();
        for( int i = 0; i < len; i++ )
        {
            unload( m_plugins[i] );
        }
    }

    void EvalPluginManager::getPluginInfo( const CString& arxFilePath, EvalPluginInfo& info )
    {
        int len = m_plugins.length();
        for( int i = 0; i < len; i++ )
        {
            if ( arxFilePath.CompareNoCase( m_plugins[i].arxFilePath ) == 0 )
            {
                m_plugins[i].pEvalPlugin->getPluginInfo( info );
                break;
            }
        }
    }

    void EvalPluginManager::getAllLoadedPluginPaths( AcStringArray& paths )
    {
        int len = m_plugins.length();
        for( int i = 0; i < len; i++ )
        {
            paths.append( m_plugins[i].arxFilePath );
        }
    }
}