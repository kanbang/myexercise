
// FindFileTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFindFileTestApp:
// �йش����ʵ�֣������ FindFileTest.cpp
//

class CFindFileTestApp : public CWinAppEx
{
public:
    CFindFileTestApp();

// ��д
public:
    virtual BOOL InitInstance();

// ʵ��

    DECLARE_MESSAGE_MAP()
};

extern CFindFileTestApp theApp;