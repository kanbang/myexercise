// AFC.h : AFC DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAFCApp
// �йش���ʵ�ֵ���Ϣ������� AFC.cpp
//

class CAFCApp : public CWinApp
{
public:
	CAFCApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
