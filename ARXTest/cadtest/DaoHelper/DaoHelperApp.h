// DaoHelper.h : DaoHelper DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDaoHelperApp
// �йش���ʵ�ֵ���Ϣ������� DaoHelper.cpp
//

class CDaoHelperApp : public CWinApp
{
public:
	CDaoHelperApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
