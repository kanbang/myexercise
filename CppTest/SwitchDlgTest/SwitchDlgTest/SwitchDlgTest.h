
// SwitchDlgTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSwitchDlgTestApp:
// �йش����ʵ�֣������ SwitchDlgTest.cpp
//

class CSwitchDlgTestApp : public CWinAppEx
{
public:
    CSwitchDlgTestApp();

// ��д
public:
    virtual BOOL InitInstance();

// ʵ��

    DECLARE_MESSAGE_MAP()
};

extern CSwitchDlgTestApp theApp;