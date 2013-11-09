#pragma once

#include "dlimexp.h"

class ArxToolBar;

class ARXUI_DLLIMPEXP AppToolBar
{
public:
	AppToolBar(void);
	~AppToolBar(void);

	// ��ӹ�����
	void AddToolBar(const TCHAR* toolBarName);
	// ��Ӱ�ť��buttonName-�˵���"--"Ϊ�ָ���,commandҪ�ӿո�
	void AddToolButton(const TCHAR* name, const TCHAR* command, const TCHAR* help = NULL, const TCHAR* bmp= NULL);
	// ��ӷָ���
	void AddSeparator();

	// ����������
	// bmpDir -- ͼ�����ڵ��ļ���·��
	void CreateToolBars(const TCHAR* bmpDir);

private:
	// ɾ��������
	void CleanUpToolBars(); //������Menuһ��Ҫ�����������

private:
	ArxToolBar* m_pToolBar;
};
