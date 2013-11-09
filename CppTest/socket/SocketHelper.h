#pragma once

#include "afxwin.h"
#include "afxsock.h"

#define CS_IP_ADDRESS "127.0.0.1"
#define CS_PORT 10000

extern CSocket* InitServer(UINT Port);
extern CSocket* AcceptSocket(CSocket* pServerSocket);
extern CSocket* InitClient(const CString& Ipaddress, UINT Port);
extern bool SendData(CSocket* pSocket, CObject* pObj);
extern bool RecvData(CSocket* pSocket, CObject* pObj);