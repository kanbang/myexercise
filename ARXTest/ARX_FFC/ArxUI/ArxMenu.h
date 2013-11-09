/*-------------------------------------------------------------------------
作者： Racky Ye
该类封装了在AuToCAD上添加菜单及子菜单的方法，使用COM接口实现
大家可以任意修改，添加新功能，希望大家能多交流
Email: yeguixi@126.com 
--------------------------------------------------------------
使用方法：
ArxMenu menu;
menu.AddMenu("我的菜单");
menu.AddSubMenu("弹出对话框", "_DLG ");
menu.AddSubMenu("绘制线条", "_DRAWL ");
menu.AddSubMenu("*Line");// 加"*"号表示是二级子菜单
menu.AddSubMenu("$CIRCLE", "_CIRCLE ");//加"$"号表示二级子菜单下的菜单条
menu.AddSubSeparator();
menu.AddSubMenu("$PLINE", "_PLINE ");//加"$"号表示二级子菜单下的菜单条
menu.AddSeparator();
menu.AddSubMenu("选择文件夹", "_SELFOLDER ");
menu.AddSubMenu("*TEXT");// 加"*"号表示是二级子菜单
menu.AddSubMenu("$MTEXT", "_TEXT ");//加"$"号表示二级子菜单下的菜单条
menu.CreateMenus();
--------------------------------------------------------------
----------------------------------------------------------------------------*/

#pragma once

#include "acadi.h"

// 菜单项
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

	// 添加菜单
	void AddMenu(const TCHAR* menuName);
	// 添加子菜单，menuName-菜单名"--"为分隔符,command要加空格
	void AddSubMenu(const TCHAR* menuName, const TCHAR* command=NULL);
	// 添加分隔符
	void AddSeparator();
	// 添加二级子菜单分隔符
	void AddSubSeparator();

	// 创建菜单
	void CreateMenus();
	// 删除菜单
	void CleanUpMenus(); //创建了Menu一定要调用这个函数

	static bool IsMenuExist(IAcadPopupMenu *&pMnu,long &index,LPCTSTR mnuname,IAcadPopupMenus *mnus);//菜单是否存在，避免二次加载 add by zhengzhilin
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
	int m_menuCount;  // 菜单个数

	CStringArray m_TopMenuNames; // 菜单名
};
