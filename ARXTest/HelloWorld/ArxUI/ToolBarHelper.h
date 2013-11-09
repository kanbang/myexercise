#pragma once

#include "dlimexp.h"

class AppToolBar;

class ARXUI_DLLIMPEXP ToolBarHelper
{
public:
	ToolBarHelper(void);
	~ToolBarHelper(void);

	// ��ӹ�����
	void AddToolBar(const TCHAR* toolBarName);
	// ��Ӱ�ť��buttonName-�˵���"--"Ϊ�ָ���,commandҪ�ӿո�
	void AddToolButton(const TCHAR* name, const TCHAR* command, const TCHAR* help = NULL, const TCHAR* bmp= NULL);
	// ��ӷָ���
	void AddSeparator();

	// ����������
	void CreateToolBars();

	// ɾ��������
	static void CleanUpToolBars(); //������Menuһ��Ҫ�����������

private:
	AppToolBar* m_pToolBarImpl;
};
