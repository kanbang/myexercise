// ThreadAsynTeamer.cpp: implementation of the CThreadAsynTeamer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PubFun.h"
#include "ThreadAsynTeamwork.h"


class CThreadManager
{
private:
    CThreadManager();
    ~CThreadManager();

    typedef vector<HANDLE>		TVecThread;
    typedef map<HANDLE, DWORD>  TMapThread;
    typedef void ( *FnCallback )( DWORD dwThreadId );

private:
    inline static CThreadManager* Instance()
    {
        if ( m_pThreadManager == NULL )
        {
            m_pThreadManager = new CThreadManager;
        }

        return m_pThreadManager;
    }

    inline static void Release()
    {
        if ( m_pThreadManager != NULL )
        {
            delete m_pThreadManager;
        }

        m_pThreadManager = NULL;
    }

    static DWORD WINAPI ThreadProc( LPVOID pParam );

    int AddListen( DWORD dwThreadId );

    void SetCallback( FnCallback fnCallback );

private:
    static CThreadManager* m_pThreadManager;

    TVecThread   m_vThread;				//>>> ��������̶߳���
    TMapThread   m_mapThread;			//>>> �߳�ID�;����Ӧ��ϵ
    FnCallback   m_fnCallback;          //>>> һ���߳̽���ʱҪ�ص��ĺ���

    CRITICAL_SECTION   m_cs;			//>>>  �ٽ���
    HANDLE       m_hd;                  //>>>  �����̵߳ľ��
    friend class CThreadAsynTeamwork;
};

CThreadManager* CThreadManager::m_pThreadManager = NULL;

CThreadManager::CThreadManager()
{
    m_pThreadManager = NULL;
    m_fnCallback = NULL;
    ::InitializeCriticalSection( &m_cs );

    //�¼�1�������µĴ���Ƕ��ʱ��������
    HANDLE hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
    //�¼�2�����ƹ����߳��Ƿ����
    HANDLE hEvent1 = CreateEvent( NULL, FALSE, FALSE, NULL );

    m_vThread.push_back( hEvent );
    m_vThread.push_back( hEvent1 );

    DWORD dwThreadId;
    m_hd = chBEGINTHREADEX( NULL, 0, ThreadProc, NULL, 0, &dwThreadId );
}

CThreadManager::~CThreadManager()
{
    SetEvent( m_vThread[1] ); //֪ͨ�߳̽���
    ::WaitForSingleObject( m_hd, 3000 );

    for ( int i = 0; i < m_vThread.size(); i++ )
    {
        ::CloseHandle( m_vThread[i] );
    }
    m_vThread.clear();
    m_mapThread.clear();
    ::DeleteCriticalSection( &m_cs );
}

void CThreadManager::SetCallback( FnCallback fnCallback )
{
    CCSLock lock( &m_cs );
    m_fnCallback = fnCallback;
}

int CThreadManager::AddListen( DWORD dwThreadId )
{
    HANDLE hThread = OpenThread( THREAD_ALL_ACCESS, FALSE, dwThreadId );
    if ( hThread == NULL )
        return 1;

    CCSLock lock( &m_cs );
    m_vThread.push_back( hThread );
    m_mapThread.insert( make_pair( hThread, dwThreadId ) );
    ::SetEvent( m_vThread[0] );

    return 0;
}

DWORD WINAPI CThreadManager::ThreadProc( LPVOID pParam )
{
    BOOL bRun = TRUE;

    while ( bRun )
    {
        CCSLock lock( &m_pThreadManager->m_cs );
        int nCnt = m_pThreadManager->m_vThread.size();
        lock.UnLock();

        DWORD dwRet = ::WaitForMultipleObjects( nCnt, &m_pThreadManager->m_vThread[0], FALSE, INFINITE );

        if ( WAIT_OBJECT_0 == dwRet )
        {
            continue;
        }
        else if ( WAIT_OBJECT_0 + 1 == dwRet )
        {
            bRun = FALSE;
        }
        else
        {
            ASSERT( ( dwRet > WAIT_OBJECT_0 + 1 ) && ( dwRet < m_pThreadManager->m_vThread.size() ) );

            lock.Lock( &m_pThreadManager->m_cs );
            HANDLE hThread = m_pThreadManager->m_vThread[dwRet - WAIT_OBJECT_0];

            //���̹߳����б����Ƴ�
            TVecThread::iterator itVec = find( m_pThreadManager->m_vThread.begin(),
                                               m_pThreadManager->m_vThread.end(), hThread );

            if ( itVec != m_pThreadManager->m_vThread.end() )
            {
                m_pThreadManager->m_vThread.erase( itVec );
            }

            //һ���߳��ѽ������ص�
            TMapThread::iterator itMap = m_pThreadManager->m_mapThread.find( hThread );
            if ( itMap != m_pThreadManager->m_mapThread.end() )
            {
                if ( m_pThreadManager->m_fnCallback != NULL )
                {
                    m_pThreadManager->m_fnCallback( itMap->second );
                }
                m_pThreadManager->m_mapThread.erase( itMap );
            }

            ::CloseHandle( hThread );
            lock.UnLock();
        }
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

CThreadAsynTeamwork* CThreadAsynTeamwork::m_pThreadAsyn = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadAsynTeamwork::CThreadAsynTeamwork()
{
    m_pThreadAsyn = NULL;
    WM_ASYNNOTIFYMESSAGE = ::RegisterWindowMessage( "CThreadAsynTeamwork.NotifyMessage" );
    ::InitializeCriticalSection( &m_cs );
    CThreadManager::Instance()->SetCallback( ThreadDetach );
}

CThreadAsynTeamwork::~CThreadAsynTeamwork()
{
    ::DeleteCriticalSection( &m_cs );
    CThreadManager::Release();
    m_mapThreadId.clear();
    m_mapThreadCallback.clear();
}

int CThreadAsynTeamwork::SetCallBackProc( DWORD dwChild,
        FnCallbackProc fnCallback, BOOL bAutoCallback /*= FALSE*/ )
{
    CCSLock lock( &m_cs );

    DWORD dwThreadId = GetCurrentThreadId();
    TMapThreadCallback::iterator itMap = m_mapThreadCallback.find( dwThreadId );

    if ( itMap != m_mapThreadCallback.end() ) //�ҵ��˸��߳�,�ѹҹ�
    {
        vector<ThreadInfo>& vecThreadInfo = itMap->second.vThreadInfo;
        vector<ThreadInfo>::iterator itVec;
        for ( itVec = vecThreadInfo.begin(); itVec != vecThreadInfo.end(); itVec++ )
        {
            if ( dwChild == ( *itVec ).dwChildId ) //���߳������ûص�
            {
                if ( fnCallback != NULL ) //�ص�������ַ��Ч
                {
                    ( *itVec ).bAutoCallback = bAutoCallback;
                    ( *itVec ).fnCallback = fnCallback;

                    return 0;
                }

                //�Ƴ������̵߳Ļص�����
                vecThreadInfo.erase( itVec );
                if ( vecThreadInfo.size() == 0 ) //û�����߳����ûص�,������̹߳ҹ�����map���Ƴ�
                {
                    ::UnhookWindowsHookEx( itMap->second.hHook );
                    m_mapThreadCallback.erase( itMap );
                }
                TMapThreadId::iterator itMapthId = m_mapThreadId.find( dwChild );
                if ( itMapthId != m_mapThreadId.end() )
                {
                    m_mapThreadId.erase( itMapthId ); //�Ƴ����̺߳͸��̹߳�ϵ�б�
                }

                return 0;
            }
        }

        //���߳��ѹҹ��������߳��б���û�����ù��ص���Ϣ
        if ( fnCallback != NULL )
        {
            ThreadInfo thInfo;
            thInfo.dwChildId = dwChild;
            thInfo.bAutoCallback = bAutoCallback;
            thInfo.fnCallback = fnCallback;

            vecThreadInfo.push_back( thInfo );
            m_mapThreadId.insert( make_pair( dwChild, dwThreadId ) );
            CThreadManager::Instance()->AddListen( dwChild );
        }

        return 0;
    }

    if ( fnCallback == NULL )
        return 0;


    HHOOK hHook = ::SetWindowsHookEx( WH_GETMESSAGE, ( HOOKPROC ) GetMsgProc,
                                      GetModuleHandle( NULL ), dwThreadId );

    if ( hHook != NULL ) //��װ�ҹ��ɹ�
    {
        ThreadInfo thInfo;
        thInfo.dwChildId = dwChild;
        thInfo.bAutoCallback = bAutoCallback;
        thInfo.fnCallback = fnCallback;

        ThreadCallback thCallback;
        thCallback.hHook = hHook;
        thCallback.vThreadInfo.push_back( thInfo );
        m_mapThreadCallback.insert( make_pair( dwThreadId, thCallback ) );

        m_mapThreadId.insert( make_pair( dwChild, dwThreadId ) );
        CThreadManager::Instance()->AddListen( dwChild );
        return 0;
    }

    return 1;
}

LRESULT CALLBACK CThreadAsynTeamwork::GetMsgProc( int nCode, WPARAM wParam, LPARAM lParam )
{
    CCSLock lock( &m_pThreadAsyn->m_cs );
    DWORD dwThreadId = GetCurrentThreadId();
    TMapThreadCallback::iterator itMap = m_pThreadAsyn->m_mapThreadCallback.find( dwThreadId );

    if ( itMap == m_pThreadAsyn->m_mapThreadCallback.end() ) //�߳��ѹҹ�����map��û���ҵ�
    {
        ASSERT( FALSE );
        return 0;
    }
    HHOOK hHook = itMap->second.hHook;
    lock.UnLock();

    PMSG pMsg = ( PMSG )lParam;
    if ( nCode == HC_ACTION && wParam == PM_REMOVE &&
            pMsg->message == m_pThreadAsyn->WM_ASYNNOTIFYMESSAGE )
    {
        PParamInfo pParam = ( PParamInfo )pMsg->wParam;
        CSmartDel<ParamInfo> smartDel( pParam );

        FnCallbackProc fnCallback = NULL;

        //��ȡ�ص�������ַ
        lock.Lock( &m_pThreadAsyn->m_cs );
        vector<ThreadInfo>& vecThInfo = itMap->second.vThreadInfo;
        vector<ThreadInfo>::iterator itVec;
        for ( itVec = vecThInfo.begin(); itVec != vecThInfo.end(); itVec++ )
        {
            if ( ( *itVec ).dwChildId == pParam->dwThreadId )
            {
                fnCallback = ( *itVec ).fnCallback;
                break;
            }
        }
        lock.UnLock();

        if ( fnCallback != NULL && pParam->bNotify )
        {
            fnCallback( ( PVOID )pMsg->lParam );
        }

        if ( pParam->bDelete ) //������
        {
            lock.Lock( &m_pThreadAsyn->m_cs );
            if ( itVec != vecThInfo.end() ) //�����߳��б����Ƴ�
                vecThInfo.erase( itVec );

            if ( vecThInfo.size == 0 )
            {
                ::UnhookWindowsHookEx( hHook );
                m_pThreadAsyn->m_mapThreadCallback.erase( itMap );
                return 0;
            }
            lock.UnLock();
        }
    }

    return CallNextHookEx( hHook, nCode, wParam, lParam );
}

BOOL CThreadAsynTeamwork::Notify( PVOID pExtra )
{
    CCSLock lock( &m_cs );
    DWORD dwThreadId = ::GetCurrentThreadId();
    TMapThreadId::iterator itMapThId = m_mapThreadId.find( dwThreadId );

    if ( itMapThId != m_mapThreadId.end() )
    {
        PParamInfo pParam = new ParamInfo;
        pParam->dwThreadId = dwThreadId;
        pParam->bDelete  = FALSE;
        pParam->bNotify  = TRUE;

        return ::PostThreadMessage( itMapThId->second, WM_ASYNNOTIFYMESSAGE,
                                    ( WPARAM )pParam, ( LPARAM )pExtra );
    }

    return FALSE;
}


void CThreadAsynTeamwork::ThreadDetach( DWORD dwThreadId )
{
    CCSLock lock( &m_pThreadAsyn->m_cs );

    TMapThreadId::iterator itMapThId = m_pThreadAsyn->m_mapThreadId.find( dwThreadId );

    if ( itMapThId == m_pThreadAsyn->m_mapThreadId.end() )
        return;

    TMapThreadCallback::iterator itMapCallback =
        m_pThreadAsyn->m_mapThreadCallback.find( itMapThId->second );

    if ( itMapCallback == m_pThreadAsyn->m_mapThreadCallback.end() )
        return;

    vector<ThreadInfo>& vecThInfo = itMapCallback->second.vThreadInfo;

    for( int i = 0; i < vecThInfo.size(); i++ )
    {
        if ( dwThreadId == vecThInfo[i].dwChildId )
        {
            PParamInfo pParam = new ParamInfo;
            pParam->dwThreadId = dwThreadId;
            pParam->bDelete  = TRUE;
            pParam->bNotify  = vecThInfo[i].bAutoCallback;
            ::PostThreadMessage( itMapThId->second, m_pThreadAsyn->WM_ASYNNOTIFYMESSAGE,
                                 ( WPARAM )pParam, ( LPARAM )NULL );
            break;
        }
    }
}