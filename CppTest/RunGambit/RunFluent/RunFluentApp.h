
// RunFluent.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRunFluentApp:
// �йش����ʵ�֣������ RunFluent.cpp
//

class CRunFluentApp : public CWinAppEx
{
public:
    CRunFluentApp();

// ��д
public:
    virtual BOOL InitInstance();

// ʵ��

    DECLARE_MESSAGE_MAP()
};

extern CRunFluentApp theApp;