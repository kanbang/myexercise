#include "StdAfx.h"
#include "SocketHelper.h"

struct CS_Data
{
    CObject* m_pObj;
    int m_bInit;

    void Serialize( CArchive& ar )
    {
        m_pObj->Serialize( ar );

        if( ar.IsStoring() )
        {
            ar << m_bInit;
        }
        else
        {
            ar >> m_bInit;
        }
    }
};

CSocket* InitServer( UINT Port )
{
    AfxMessageBox( "初始化服务器" );

    if ( !AfxSocketInit() )
    {
        AfxMessageBox( "Soket初始化失败! " );
        return 0;
    }

    CSocket* pServerSocket = new CSocket;

    if ( !pServerSocket->Create( Port ) )
    {
        AfxMessageBox( "创建soket失败" );
        return 0;
    }
    else
    {
        pServerSocket->Listen();
        AfxMessageBox( "监听端口" );
    }

    return pServerSocket;
}

CSocket* AcceptSocket( CSocket* pServerSocket )
{
    CSocket* pAcceptSocket = new CSocket;
    return pServerSocket->Accept( *pAcceptSocket ) ? pAcceptSocket : 0;
}

CSocket* InitClient( const CString& Ipaddress, UINT Port )
{
    AfxMessageBox( "OnConnect()" );

    if ( !AfxSocketInit() )
    {
        AfxMessageBox( "客户端--WindowScoket initial failed!" );
        return 0;
    }

    CSocket* pClientSocket = new CSocket;

    if ( !pClientSocket->Create() )
    {
        AfxMessageBox( "客户端--Receivesocket create failed" );
        return 0;
    }
    else
    {
        if ( pClientSocket->Connect( Ipaddress, Port ) )
        {
            AfxMessageBox( "客户端--成功" );
            return pClientSocket;
        }
        else
        {
            return 0;
        }
    }
}

static bool SendData_Helper( CSocket* pSocket, CS_Data& data )
{
    CSocketFile m_socketfile( pSocket );
    CArchive ar( &m_socketfile, CArchive::store );

    data.Serialize( ar );

    ar.Close();
    m_socketfile.Close();

    return true;
}

static bool RecvData_Helper( CSocket* pSocket, CS_Data& data )
{
    CSocketFile m_socketfile( pSocket );
    CArchive ar( &m_socketfile, CArchive::load );

    data.Serialize( ar );

    ar.Close();
    m_socketfile.Close();

    return true;
}

bool SendData( CSocket* pSocket, CObject* pObj )
{
    CS_Data data = {pObj, 1};
    SendData_Helper( pSocket, data );
    return true;
}

bool RecvData( CSocket* pSocket, CObject* pObj )
{
    CS_Data data = {pObj, 0};
    while( data.m_bInit == 0 )
    {
        RecvData_Helper( pSocket, data );
    }
    return true;
}
