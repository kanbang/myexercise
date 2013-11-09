/*-------------------------------------------------------------------------
���ߣ� Racky Ye
�����װ����AuToCAD����Ӳ˵����Ӳ˵��ķ�����ʹ��COM�ӿ�ʵ��
��ҿ��������޸ģ�����¹��ܣ�ϣ������ཻܶ��
Email: yeguixi@126.com 
--------------------------------------------------------------
ʹ�÷�����
ArxMenu menu;
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
--------------------------------------------------------------
----------------------------------------------------------------------------*/

#pragma once

#include "acadi.h"

// �˵���
struct CADMenu
{
	TCHAR name[256];
	TCHAR order[256];
};

class ArxMenu
{
public:
	ArxMenu();
	virtual ~ArxMenu();

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
	void CleanUpMenus(); //������Menuһ��Ҫ�����������

	static bool IsMenuExist(IAcadPopupMenu *&pMnu,long &index,LPCTSTR mnuname,IAcadPopupMenus *mnus);//�˵��Ƿ���ڣ�������μ��� add by zhengzhilin
	static bool GetAcadApplication(IAcadApplication *&pAcadApp);
	//static bool GetAcadMenuGroup(IAcadMenuGroup  **pVal,LPCTSTR menuname);
	static bool GetLastMenuFromMenubar(CString &sMenu);
	static void ClearMenu(IAcadPopupMenus *mnus,LPCTSTR menuname);
	static bool GetAcadApplication(LPDISPATCH * pVal);
	static bool GetAcadMenuGroup(IAcadMenuGroup  **pVal, LPCTSTR menuname);
	
private:

	class CCADMenuArray : public CArray <CADMenu*, CADMenu*>
	{
	public:
		int AddMenu(CADMenu& menu);
		int AddMenu(const TCHAR* menuName, const TCHAR* command);
		~CCADMenuArray();
	};

	CArray <CCADMenuArray*, CCADMenuArray*> m_menus;
	int m_menuCount;  // �˵�����

	CStringArray m_TopMenuNames; // �˵���
};
