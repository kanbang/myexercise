
// LemonTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLemonTestApp:
// �йش����ʵ�֣������ LemonTest.cpp
//

class CLemonTestApp : public CWinAppEx
{
public:
    CLemonTestApp();

// ��д
public:
    virtual BOOL InitInstance();

// ʵ��

    DECLARE_MESSAGE_MAP()
};

extern CLemonTestApp theApp;