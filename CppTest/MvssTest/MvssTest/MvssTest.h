
// MvssTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMvssTestApp:
// �йش����ʵ�֣������ MvssTest.cpp
//

class CMvssTestApp : public CWinAppEx
{
public:
    CMvssTestApp();

// ��д
public:
    virtual BOOL InitInstance();

// ʵ��

    DECLARE_MESSAGE_MAP()
};

extern CMvssTestApp theApp;