// Test_Dialog.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTest_DialogApp:
// �йش����ʵ�֣������ Test_Dialog.cpp
//

class CTest_DialogApp : public CWinApp
{
public:
    CTest_DialogApp();

// ��д
public:
    virtual BOOL InitInstance();

// ʵ��



    DECLARE_MESSAGE_MAP()
};

extern CTest_DialogApp theApp;