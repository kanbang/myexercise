#include "StdAfx.h"
#include "MineGEDrawSystem.h"
#include "MineGEDrawManager.h"
#include "MineGEPluginManager.h"
#include "MineGEDraw.h"
#include "MineGEDraw_Jig.h"

// ��ʼ����̬����
MineGEDrawSystem* MineGEDrawSystem::m_pMineGEService = 0;

MineGEDrawSystem::MineGEDrawSystem( void )
{
    m_pMineGEDrawManager = new MineGEDrawManager();
    m_pMineGEPluginManager = new MineGEPluginManager( m_pMineGEDrawManager );
}

MineGEDrawSystem::~MineGEDrawSystem( void )
{
    // �Զ�ж�����в��
    delete m_pMineGEPluginManager;
    delete m_pMineGEDrawManager;
}

MineGEDrawSystem* MineGEDrawSystem::CreateInstance()
{
    if( m_pMineGEService == 0 )
    {
        m_pMineGEService = new MineGEDrawSystem();
    }
    return m_pMineGEService;
}

MineGEDrawSystem* MineGEDrawSystem::GetInstance()
{
    return m_pMineGEService;
}

void MineGEDrawSystem::DestroyInstance()
{
    delete m_pMineGEService;
    // ����Ҫ�����һ�д��룬����m_pMineGEServiceָ��δ֪
    // ����ʱ��������
    m_pMineGEService = 0;
}

void MineGEDrawSystem::loadPlugin( const CString& arxFilePath )
{
    TCHAR szPath[_MAX_PATH] = {0}, szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFile[_MAX_FNAME], szExt[_MAX_EXT];
    _tsplitpath( ( LPCTSTR )arxFilePath, szDrive, szDir, szFile, szExt ); // ���·��
    //_tcscat_s(szPath, szDrive); // �ϲ��̷�
    //_tcscat_s(szPath, szDir);  // �ϲ�Ŀ¼
    //_tcscat_s(szPath, EXTRA_PARAM_CONFIG_FILE_NAME);  // ���츽�Ӳ����ļ�·��
    CString serviceName;
    serviceName.Format( _T( "%s_SERVICE_NAME" ), CString( szFile ).MakeUpper() );
    m_pMineGEPluginManager->load( arxFilePath, serviceName );
}

void MineGEDrawSystem::unloadPlugin( const CString& arxFilePath )
{
    m_pMineGEPluginManager->unload( arxFilePath );
}

MineGEDraw* MineGEDrawSystem::getDefaultGEDraw( const CString& geType )
{
    return m_pMineGEDrawManager->getCurrentGEDraw( geType );
}

MineGEDraw* MineGEDrawSystem::getGEDraw( const CString& geType, const CString& realGEDrawName )
{
    return m_pMineGEDrawManager->getGEDraw( geType, realGEDrawName );
}

bool MineGEDrawSystem::setCurrentGEDraw( const CString& geType, const CString& realGEDrawName )
{
    return m_pMineGEDrawManager->setCurrentGEDraw( geType, realGEDrawName );
}

void MineGEDrawSystem::getAllGEDrawsByGEType( const CString& geType, AcStringArray& drawList )
{
    m_pMineGEDrawManager->getAllGEDrawsByGEType( geType, drawList );
}

MineGEDraw_Jig* MineGEDrawSystem::getGEDraw_Jig( const CString& geType, const CString& realGEDrawName )
{
    return m_pMineGEDrawManager->getGEDraw_Jig( geType, realGEDrawName );
}

CString MineGEDrawSystem::getDefaultGEDrawName( const CString& geType )
{
    MineGEDraw* pDraw = getDefaultGEDraw( geType );
    if( pDraw == 0 ) return _T( "" );
    return CString( pDraw->isA()->name() );
}