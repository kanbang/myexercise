/*
ʹ�÷�����
MenuHelper menu;
menu.AddMenu("�ҵĲ˵�");
menu.AddSubMenu("�����Ի���", "_DLG ");
menu.AddSubMenu("��������", "_DRAWL ");
menu.AddSubMenu("*Line");// ��"*"�ű�ʾ�Ƕ����Ӳ˵�
menu.AddSubMenu("$CIRCLE", "_CIRCLE ");//��"$"�ű�ʾ�����Ӳ˵��µĲ˵���
menu.AddSubSeparator();
menu.AddSubMenu("$PLINE", "_PLINE ");//��"$"�ű�ʾ�����Ӳ˵��µĲ˵���
menu.AddSeparator();
menu.AddSubMenu("ѡ���ļ���", "_SELFOLDER ");
menu.AddSubMenu("*TEXT");// ��"*"�ű�ʾ�Ƕ����Ӳ˵�
menu.AddSubMenu("$MTEXT", "_TEXT ");//��"$"�ű�ʾ�����Ӳ˵��µĲ˵���
menu.CreateMenus();
*/

#pragma once

#include "dlimexp.h"

class AppMenu;

class ARXUI_DLLIMPEXP MenuHelper
{
public:
	MenuHelper(void);
	~MenuHelper(void);

	// ��Ӳ˵�
	void AddMenu(const TCHAR* menuName);
	// ����Ӳ˵���menuName-�˵���"--"Ϊ�ָ���,commandҪ�ӿո�
	void AddSubMenu(const TCHAR* menuName, const TCHAR* command=NULL);
	// ��ӷָ���
	void AddSeparator();
	// ��Ӷ����Ӳ˵��ָ���
	void AddSubSeparator();

	// �����˵�
	void CreateMenus();

	// ɾ���˵�
	//������Menuһ��Ҫ�����������
	static void CleanUpMenus();

private:
	AppMenu* m_pMenuImpl;
};
