// MyDll.h : MyDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyDllApp
// �йش���ʵ�ֵ���Ϣ������� MyDll.cpp
//

class CMyDllApp : public CWinApp
{
public:
    CMyDllApp();

// ��д
public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};
