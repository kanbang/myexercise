
// ExpandDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CExpandDlgApp:
// �йش����ʵ�֣������ ExpandDlg.cpp
//

class CExpandDlgApp : public CWinAppEx
{
public:
    CExpandDlgApp();

// ��д
public:
    virtual BOOL InitInstance();

// ʵ��

    DECLARE_MESSAGE_MAP()
};

extern CExpandDlgApp theApp;